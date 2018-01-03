#include "comhandler.h"
#include "converter.h"

void COMHandler::configureCOM(QString portName, QIODevice::OpenModeFlag openModeFlag)
{
    com = new QSerialPort(portName);
    com->setBaudRate(QSerialPort::Baud9600);
    com->setDataBits(QSerialPort::Data8);
    com->setParity(QSerialPort::NoParity);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setStopBits(QSerialPort::OneStop);
    com->open(openModeFlag);
    qDebug() << "COM-порт готов к работе";
    name = portName;
}

void COMHandler::finishCOM()
{
    com->flush();
    com->close();
}



void COMHandler::append(QByteArray *bytes, double d)
{
    QByteArray doubleBytes = TypesConverter::toByteArray(d);
    for (int i = 0; i < 8; i++) {
        append(bytes, (byte)doubleBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, float f)
{
    QByteArray floatBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        append(bytes, (byte)floatBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, unsigned short f)
{
    QByteArray shortBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 2; i++) {
        append(bytes, (byte)shortBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, unsigned int f)
{
    QByteArray intBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        append(bytes, (byte)intBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, byte b)
{
    //qDebug() << "bytes version called";
    bytes->append(b);
    // DLE в сообщении необходимо "застаффить".
    if (b == DLE)
    {
        bytes->append(DLE);
    }
}



QString COMHandler::parse_REPORT_UNPARSABLE(QByteArray data)
{
    QString res;
    res.append("Невозможно разобрать то, что Вы отправили.\n"
               "Содержимое отправленного пакета:\n");
    for (int i = 0; i < data.length(); i++) {
        res.append(QString("%0 ").arg((byte)data[i], 1, 16));
    }
    return res;
}

QString COMHandler::parse_REPORT_DOUBLE_XYZ_POS(QByteArray data)
{
    if (data.length() != 36) {
        qDebug() << "Пакет REPORT_DOUBLE_XYZ_POS (0x83) имеет неверную длину";
        return "";
    }

    double X = TypesConverter::bytesToDouble(data, 0);
    double Y = TypesConverter::bytesToDouble(data, 8);
    double Z = TypesConverter::bytesToDouble(data, 16);
    double clockBias = TypesConverter::bytesToDouble(data, 24);
    float timeOfFix = TypesConverter::bytesToSingle(data, 32);
    return QString("Получен пакет REPORT_DOUBLE_XYZ_POS (0x83):\n \
                     --Позиция XYZ: (%0; %1; %2)\n \
                     --Clock bias: %3\n--Time of fix: %4")
            .arg(X).arg(Y).arg(Z).arg(clockBias).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_DOUBLE_LLA_POS(QByteArray data)
{
    if (data.length() != 36) {
        qDebug() << "Пакет REPORT_DOUBLE_LLA_POS (0x84) имеет неверную длину";
        return "";
    }

    double latitude = TypesConverter::bytesToDouble(data, 0);
    double longitude = TypesConverter::bytesToDouble(data, 8);
    double altitude = TypesConverter::bytesToDouble(data, 16);
    double clockBias = TypesConverter::bytesToDouble(data, 24);
    float timeOfFix = TypesConverter::bytesToSingle(data, 32);
    return QString("Получен пакет REPORT_DOUBLE_LLA_POS (0x84):\n \
                     --Позиция LLA: (%0 rad; %1 rad; %2 meters)\n \
                     --Clock bias: %3\n--Time of fix: %4")
             .arg(latitude).arg(longitude).arg(altitude).arg(clockBias).arg(timeOfFix);
}

void COMHandler::receiveText()
{
    char readed = 0;
    char reportCode = 0;
    parser = ParserStatus::Waiting_for_message;

    while (true) {
        QByteArray data;
        while(!(com->waitForReadyRead(1000)));
        //qDebug() << "started to read...";
        com->read(&readed, 1);      // First DLE
        if (!(readed == DLE && parser == ParserStatus::Waiting_for_message)) {
            qDebug() << "Ошибка разбора!";
            exit(2);
        }
        parser = ParserStatus::Reading_data;
        //qDebug() << "took first DLE...";
        com->read(&reportCode, 1);  // Report code
        //qDebug() << "took report code... " + reportCode;
        while (true) {
            com->read(&readed, 1);
            if (readed == DLE) {
                if (parser == ParserStatus::Reading_data) {     // Stuffed DLE
                    parser = ParserStatus::Found_DLE;
                }
                else if (parser == ParserStatus::Found_DLE) {
                    data.append(DLE);
                    parser = ParserStatus::Reading_data;
                }
            }
            else {
                if (readed == ETX && parser == ParserStatus::Found_DLE) {  // DLE-ETX sequence
                    break;
                }
                data.append(readed);
            }

        }
        qDebug() << QString("Finished reading... Trying to check reportCode 0x%0").arg((byte)reportCode, 1, 16);
        // На этом этапе мы имеем код пришедшего пакета
        // и всё его содержимое без стаффинга.
        QString message;
        switch ((byte)reportCode) {
        case REPORT_UNPARSABLE:
            message.append(parse_REPORT_UNPARSABLE(data));
            break;
        case REPORT_DOUBLE_XYZ_POS:
            message.append(parse_REPORT_DOUBLE_XYZ_POS(data));
            break;
        case REPORT_DOUBLE_LLA_POS:
            message.append(parse_REPORT_DOUBLE_LLA_POS(data));
            break;
        default:
            message = QString("Неизвестный пакет 0x%0").arg((byte)reportCode, 1, 16);
            break;
        }
        emit appendReceivedText(message);

        parser = ParserStatus::Waiting_for_message;
    }
}

bool COMHandler::getBoolFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toBool();
}

int COMHandler::getIntFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toInt();
}

double COMHandler::getDoubleFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toDouble();
}

void COMHandler::build_COMMAND_SET_IO_OPTIONS(QByteArray *cmd)
{
    bool ecefChecked = getBoolFromQML("eCEFcheck", "checked");
    bool llaChecked = getBoolFromQML("lLAcheck", "checked");
    bool precision = getBoolFromQML("doublePrecRB", "checked");
    bool gpsTime = getBoolFromQML("gpsTimeRB", "checked");
    bool mslChecked = getBoolFromQML("mslRB", "checked");
    bool ecefVelChecked = getBoolFromQML("ecef_vel_check", "checked");
    bool enuVelChecked = getBoolFromQML("enu_vel_check", "checked");
    bool raw_data_report_checked = getBoolFromQML("raw_data_report_check", "checked");
    bool dbhz_out_checked = getBoolFromQML("dbhz_out_RB", "checked");

    append(cmd, (byte)(
               (ecefChecked ? BIT(0) : 0) |
               (llaChecked ? BIT(1) : 0) |
               (mslChecked ? BIT(2) : 0) |
               (precision ? BIT(4) : 0)
              ));
    append(cmd, (byte)(
               (ecefVelChecked ? BIT(0) : 0) |
               (enuVelChecked ? BIT(1) : 0)
              ));
    append(cmd, (byte)(gpsTime ? 0 : BIT(0)));
    append(cmd, (byte)(
               (raw_data_report_checked ? BIT(0) : 0) |
               (dbhz_out_checked ? BIT(3) : 0)
              ));

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *cmd)
{
    double initX = getDoubleFromQML("init_x_text", "text");
    double initY = getDoubleFromQML("init_y_text", "text");
    double initZ = getDoubleFromQML("init_z_text", "text");
    append(cmd, initX);
    append(cmd, initY);
    append(cmd, initZ);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *cmd)
{
    double initLat = getDoubleFromQML("init_lat_text", "text");
    double initLong = getDoubleFromQML("init_long_text", "text");
    double initAlt = getDoubleFromQML("init_alt_text", "text");
    append(cmd, initLat);
    append(cmd, initLong);
    append(cmd, initAlt);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *cmd)
{
    byte typeOfData = (byte)getIntFromQML("typeOfDataComboBox", "currentIndex") + 2;
    byte satelliteIndex = (byte)getIntFromQML("satellite_selection_spinner1", "value");
    append(cmd, typeOfData);
    append(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void COMHandler::send_command(int code, int subcode)
{
    QByteArray cmd;
    qDebug() << code;
    qDebug() << subcode;

    cmd.append(DLE);
    // Некоторые пакеты содержат только код команды,
    // либо код команды и ее подкод.
    append(&cmd, (byte)code);
    if (subcode > 0) {
        append(&cmd, (byte)subcode);
    }
    // Для команд, которые содержат дополнительную информацию,
    // выбирается соответствующий метод.
    switch ((byte)code) {
    case COMMAND_SET_IO_OPTIONS:
        // Если == -1, то это запрос настроек, а не их установка.
        if (subcode == 0) {
            build_COMMAND_SET_IO_OPTIONS(&cmd);
        }
        break;
    case COMMAND_SATELLITE_SELECTION:
        // Индекс спутника приходит в subcode. Подкод = 0 не добавится выше,
        // поэтому это нужно сделать отдельно.
        if (subcode == 0) {
            cmd.append((byte)subcode);
        }
        break;
    case COMMAND_ACCURATE_INIT_POS_XYZ:
        build_COMMAND_ACCURATE_INIT_POS_XYZ(&cmd);
        break;
    case COMMAND_ACCURATE_INIT_POS_LLA:
        build_COMMAND_ACCURATE_INIT_POS_LLA(&cmd);
        break;
    case COMMAND_REQUEST_SATELLITE_SYSTEM_DATA:
        // Подкод здесь всегда = 1, он же - первый информационный байт
        // (см. документацию).
        build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(&cmd);
        break;
    }

    cmd.append(DLE);
    cmd.append(ETX);
    com->write(cmd.constData(), cmd.length());
    com->waitForBytesWritten(1000);
}

void COMHandler::run()
{
    (this->*methodToStartThreadWith)();
}
