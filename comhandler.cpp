#include "comhandler.h"


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



void COMHandler::appendAndStuff(QByteArray *bytes, double d)
{
    QByteArray doubleBytes = TypesConverter::toByteArray(d);
    for (int i = 0; i < 8; i++) {
        appendAndStuff(bytes, (byte)doubleBytes[i]);
    }
}

void COMHandler::appendAndStuff(QByteArray *bytes, float f)
{
    QByteArray floatBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (byte)floatBytes[i]);
    }
}

void COMHandler::appendAndStuff(QByteArray *bytes, unsigned short f)
{
    QByteArray shortBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 2; i++) {
        appendAndStuff(bytes, (byte)shortBytes[i]);
    }
}

void COMHandler::appendAndStuff(QByteArray *bytes, unsigned int f)
{
    QByteArray intBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (byte)intBytes[i]);
    }
}

void COMHandler::appendAndStuff(QByteArray *bytes, byte b)
{
    //qDebug() << "bytes version called";
    bytes->append(b);
    // DLE в сообщении необходимо "застаффить".
    if (b == DLE) {
        bytes->append(DLE);
    }
}


void COMHandler::receiveReport()
{
    char readed = 0;
    char reportCode = 0;
    parserStatus = ParserStatus::Waiting_for_message;

    while (true) {
        QByteArray data;
        while(!(com->waitForReadyRead(1000)));
        //qDebug() << "started to read...";
        com->read(&readed, 1);      // First DLE
        if (!(readed == DLE && parserStatus == ParserStatus::Waiting_for_message)) {
            qDebug() << "Ошибка разбора!";
            exit(2);
        }
        parserStatus = ParserStatus::Reading_data;
        //qDebug() << "took first DLE...";
        com->read(&reportCode, 1);  // Report code
        //qDebug() << "took report code... " + reportCode;
        while (true) {
            com->read(&readed, 1);
            if (readed == DLE) {
                if (parserStatus == ParserStatus::Reading_data) {     // Stuffed DLE
                    parserStatus = ParserStatus::Found_DLE;
                }
                else if (parserStatus == ParserStatus::Found_DLE) {
                    data.append(DLE);
                    parserStatus = ParserStatus::Reading_data;
                }
            }
            else {
                if (readed == ETX && parserStatus == ParserStatus::Found_DLE) {  // DLE-ETX sequence
                    break;
                }
                data.append(readed);
            }

        }
        qDebug() << QString("Finished reading... Trying to check reportCode 0x%0").arg((byte)reportCode, 1, 16);
        // На этом этапе мы имеем код пришедшего пакета
        // и всё его содержимое без стаффинга.
        QString message;
        PacketParser *parser = new PacketParser(data);
        switch ((byte)reportCode) {
        case REPORT_UNPARSABLE:
            message.append(parser->parse_REPORT_UNPARSABLE());
            break;
        case REPORT_DOUBLE_XYZ_POS:
            message.append(parser->parse_REPORT_DOUBLE_XYZ_POS());
            break;
        case REPORT_DOUBLE_LLA_POS:
            message.append(parser->parse_REPORT_DOUBLE_LLA_POS());
            break;
        case REPORT_SINGLE_XYZ_POS:
            message.append(parser->parse_REPORT_SINGLE_XYZ_POS());
            break;
        case REPORT_SINGLE_VELOCITY_FIX_XYZ:
            message.append(parser->parse_REPORT_SINGLE_VELOCITY_FIX_XYZ());
            break;
        case REPORT_SOFTWARE_VERSION_INFO:
            message.append(parser->parse_REPORT_SOFTWARE_VERSION_INFO());
            break;
        case REPORT_TRACKED_SATELLITES_SINGAL_LVL:
            message.append(parser->parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL());
            break;
        case REPORT_SINGLE_LLA_POS:
            message.append(parser->parse_REPORT_SINGLE_LLA_POS());
            break;
        case REPORT_REQUEST_IO_OPTIONS:
            message.append(parser->parse_REPORT_REQUEST_IO_OPTIONS());
            break;
        case REPORT_SINGLE_VELOCITY_FIX_ENU:
            message.append(parser->parse_REPORT_SINGLE_VELOCITY_FIX_ENU());
            break;
        case REPORT_GPS_SYSTEM_DATA:
            message.append(parser->parse_REPORT_GPS_SYSTEM_DATA());
            break;
        case REPORT_STATUS_SATELLITE_HEALTH:
            message.append(parser->parse_REPORT_STATUS_SATELLITE_HEALTH());
            break;
        case REPORT_RAW_MEASUREMENT_DATA:
            message.append(parser->parse_REPORT_RAW_MEASUREMENT_DATA());
            break;
        case REPORT_SATELLITE_TRACKING_STATUS:
            message.append(parser->parse_REPORT_SATELLITE_TRACKING_STATUS());
            break;
        case REPORT_SATELLITE_SELECTION_LIST:
            message.append(parser->parse_REPORT_SATELLITE_SELECTION_LIST());
            break;
        default:
            message = QString("Неизвестный пакет 0x%0").arg((byte)reportCode, 1, 16);
            break;
        }
        emit appendReceivedText(message);

        parserStatus = ParserStatus::Waiting_for_message;
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

    appendAndStuff(cmd, (byte)(
               (ecefChecked ? BIT(0) : 0) |
               (llaChecked ? BIT(1) : 0) |
               (mslChecked ? BIT(2) : 0) |
               (precision ? BIT(4) : 0)
              ));
    appendAndStuff(cmd, (byte)(
               (ecefVelChecked ? BIT(0) : 0) |
               (enuVelChecked ? BIT(1) : 0)
              ));
    appendAndStuff(cmd, (byte)(gpsTime ? 0 : BIT(0)));
    appendAndStuff(cmd, (byte)(
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
    appendAndStuff(cmd, initX);
    appendAndStuff(cmd, initY);
    appendAndStuff(cmd, initZ);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *cmd)
{
    double initLat = getDoubleFromQML("init_lat_text", "text");
    double initLong = getDoubleFromQML("init_long_text", "text");
    double initAlt = getDoubleFromQML("init_alt_text", "text");
    appendAndStuff(cmd, initLat);
    appendAndStuff(cmd, initLong);
    appendAndStuff(cmd, initAlt);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *cmd)
{
    byte typeOfData = (byte)getIntFromQML("typeOfDataComboBox", "currentIndex") + 2;
    byte satelliteIndex = (byte)getIntFromQML("satellite_selection_spinner1", "value");
    appendAndStuff(cmd, typeOfData);
    appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *cmd)
{
    byte satelliteIndex = (byte)getIntFromQML("satellites_and_health_spinner", "value");
    appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void COMHandler::send_command(int code, int subcode)
{
    QByteArray cmd;
    qDebug() << code;
    qDebug() << subcode;

    cmd.append(DLE);
    // Некоторые пакеты содержат только код команды,
    // либо код команды и ее подкод. Для таких пакетов
    // дополнительный метод не нужен.
    appendAndStuff(&cmd, (byte)code);
    if (subcode > 0) {
        appendAndStuff(&cmd, (byte)subcode);
    }
    // Для команд, которые содержат дополнительную информацию,
    // выбирается соответствующий метод.
    switch ((byte)code) {
    case COMMAND_SET_IO_OPTIONS:
        // (Если == -1, то это запрос настроек, а не их установка.)
        if (subcode == 0) {
            build_COMMAND_SET_IO_OPTIONS(&cmd);
        }
        break;
    case COMMAND_SATELLITE_SELECTION:
    case COMMAND_REQUEST_LAST_RAW_MEASUREMENT:
    case COMMAND_REQUEST_SATELLITE_TRACKING_STATUS:
        // Индекс спутника для этих команд приходит в subcode.
        // Подкод == 0 не добавится выше, поэтому это нужно сделать отдельно.
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
        // Подкод здесь всегда == 1, он же - первый информационный байт (см. документацию).
        build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(&cmd);
        break;
    case COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH:
        // Для всех возможных операций (1 - 6) номер операции уже пришел в подкоде.
        // Для всех операций также нужно добавить номер спутника, которого эта операция касается
        // (для операций 3 и 6 номер спутника не важен, однако пакет всё равно нужно дополнить).
        build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(&cmd);
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
