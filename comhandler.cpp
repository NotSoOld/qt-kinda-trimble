#include "comhandler.h"
#include <QQuickView>
#include <QQuickItem>

QSerialPort *COMHandler::com;
QString COMHandler::name;
QByteArray COMHandler::readedData;
byte COMHandler::previouslyReadedChar;

void COMHandler::configureCOM(QString portName, int baudRate, int dataBits, int parity, int flowControl, int stopBits)
{/*
    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/satellite_status_template.qml")));
    view.show();
    QQuickItem *object = view.rootObject();
    object->setX(25);
    object->setY(720);
    return;
*/
    if (com && com->isOpen()) {
        finishCOM();
    }
    com = new QSerialPort(portName);
    com->setBaudRate(baudRate);
    switch (dataBits) {
        case 0: com->setDataBits(QSerialPort::Data5); break;
        case 1: com->setDataBits(QSerialPort::Data6); break;
        case 2: com->setDataBits(QSerialPort::Data7); break;
        case 3: com->setDataBits(QSerialPort::Data8); break;
        default: com->setDataBits(QSerialPort::UnknownDataBits);
    }
    switch (parity) {
        case 0: com->setParity(QSerialPort::NoParity); break;
        case 2: com->setParity(QSerialPort::EvenParity); break;
        case 3: com->setParity(QSerialPort::OddParity); break;
        case 4: com->setParity(QSerialPort::SpaceParity); break;
        case 5: com->setParity(QSerialPort::MarkParity); break;
        default: com->setParity(QSerialPort::UnknownParity);
    }
    switch (flowControl) {
        case 0: com->setFlowControl(QSerialPort::NoFlowControl); break;
        case 1: com->setFlowControl(QSerialPort::HardwareControl); break;
        case 2: com->setFlowControl(QSerialPort::SoftwareControl); break;
        default: com->setFlowControl(QSerialPort::UnknownFlowControl);
    }
    switch (stopBits) {
        case 1: com->setStopBits(QSerialPort::OneStop); break;
        case 2: com->setStopBits(QSerialPort::OneAndHalfStop); break;
        case 3: com->setStopBits(QSerialPort::TwoStop); break;
        default: com->setStopBits(QSerialPort::UnknownStopBits);
    }
    com->open(QSerialPort::ReadWrite);
    com->clear();
    name = portName;
    QObject::connect(
                com,
                &QSerialPort::readyRead,
                this,
                &COMHandler::readFromCOM
    );

    emit appendReceivedText(QString("VirtualCOM %0 подключен с параметрами: %1").arg(portName).arg(baudRate));
}

void COMHandler::finishCOM()
{
    com->clear();
    com->close();
    delete com;
}


void COMHandler::readFromCOM()
{
    int bytesAvailable = com->bytesAvailable();
    for (int i = 0; i < bytesAvailable; i++) {
        char readedChar;
        com->getChar(&readedChar);
        //qDebug() << QString("reading 0x%0, prev read 0x%1").arg((byte)readedChar, 1, 16).arg(previouslyReadedChar, 1, 16);
        if (previouslyReadedChar == DLE && (byte)readedChar == ETX) {
            // Обнаружен конец пакета. ETX также отбрасываем.
            //qDebug() << readedData;
            receiveReport();
            // После вызова этой функции всё содержимое readedData было разобрано
            // (оно содержало полный пакет), поэтому теперь можно его очистить.
            readedData.clear();
           // qDebug() << "cleared readed data";

        }

        else if (!(previouslyReadedChar == DLE && (byte)readedChar == DLE)) {
            // Если обнаружен стаффинг байта DLE, не нужно заносить его в полученные данные.
            readedData.append(readedChar);
        }

        previouslyReadedChar = (byte)readedChar;
    }
}


void COMHandler::receiveReport()
{
    // При вызове этой функции мы имеем данные пришедшего пакета
    // без стаффинга, но с начальным DLE и конечным DLE.
    // Конструктор парсера пакетов сам отбросит всё лишнее.
    QString message = "";
    PacketParser *parser = new PacketParser(readedData);
    switch (parser->reportCode()) {
    case REPORT_UNPARSABLE:
        message.append(parser->parse_REPORT_UNPARSABLE());
        break;
    case REPORT_FIRMWARE_INFO:
        switch (parser->reportSubcode()) {
        case RPTSUB_FIRMWARE_VERSION:
            message.append(parser->parse_RPTSUB_FIRMWARE_VERSION());
            break;
        case RPTSUB_HARDWARE_COMPONENT_INFO:
            message.append(parser->parse_RPTSUB_HARDWARE_COMPONENT_INFO());
            break;
        default:
            message.append(QString("Это точно был пакет REPORT_FIRMWARE_INFO (0х1С)? Подкод %0 не распознан.")
                           .arg(parser->reportSubcode(), 1, 16));
        }
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
        message.append(parser->parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL(window));
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
    case REPORT_SUPER:
        switch (parser->reportSubcode()) {
        case RPTSUB_PRIMARY_TIMING_PACKET:
            message.append(parser->parse_RPTSUB_PRIMARY_TIMING_PACKET());
            break;
        case RPTSUB_SUPPL_TIMING_PACKET:
            message.append(parser->parse_RPTSUB_SUPPL_TIMING_PACKET());
            parser->updateInterfaceValues(window);
            break;
        default:
            message.append(QString("Это точно был пакет REPORT_SUPER (0х8F)? Подкод %0 не распознан.")
                           .arg(parser->reportSubcode(), 1, 16));
        }
        break;
    default:
        message = QString("Неизвестный пакет 0x%0").arg(parser->reportCode(), 1, 16);
    }
    emit appendReceivedText(message);
}


void COMHandler::build_COMMAND_SET_IO_OPTIONS(QByteArray *cmd)
{
    bool ecefChecked = QMLDataHelper::getBoolFromQML(window, "eCEFcheck", "checked");
    bool llaChecked = QMLDataHelper::getBoolFromQML(window, "lLAcheck", "checked");
    bool precision = QMLDataHelper::getBoolFromQML(window, "doublePrecRB", "checked");
    bool gpsTime = QMLDataHelper::getBoolFromQML(window, "gpsTimeRB", "checked");
    bool mslChecked = QMLDataHelper::getBoolFromQML(window, "mslRB", "checked");
    bool ecefVelChecked = QMLDataHelper::getBoolFromQML(window, "ecef_vel_check", "checked");
    bool enuVelChecked = QMLDataHelper::getBoolFromQML(window, "enu_vel_check", "checked");
    bool raw_data_report_checked = QMLDataHelper::getBoolFromQML(window, "raw_data_report_check", "checked");
    bool dbhz_out_checked = QMLDataHelper::getBoolFromQML(window, "dbhz_out_RB", "checked");

    QByteArrayHelper::appendAndStuff(cmd, (byte)(
               (ecefChecked ? BIT(0) : 0) |
               (llaChecked ? BIT(1) : 0) |
               (mslChecked ? BIT(2) : 0) |
               (precision ? BIT(4) : 0)
              ));
    QByteArrayHelper::appendAndStuff(cmd, (byte)(
               (ecefVelChecked ? BIT(0) : 0) |
               (enuVelChecked ? BIT(1) : 0)
              ));
    QByteArrayHelper::appendAndStuff(cmd, (byte)(gpsTime ? 0 : BIT(0)));
    QByteArrayHelper::appendAndStuff(cmd, (byte)(
               (raw_data_report_checked ? BIT(0) : 0) |
               (dbhz_out_checked ? BIT(3) : 0)
              ));

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *cmd)
{
    double initX = QMLDataHelper::getDoubleFromQML(window, "init_x_text", "text");
    double initY = QMLDataHelper::getDoubleFromQML(window, "init_y_text", "text");
    double initZ = QMLDataHelper::getDoubleFromQML(window, "init_z_text", "text");
    QByteArrayHelper::appendAndStuff(cmd, initX);
    QByteArrayHelper::appendAndStuff(cmd, initY);
    QByteArrayHelper::appendAndStuff(cmd, initZ);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *cmd)
{
    double initLat = QMLDataHelper::getDoubleFromQML(window, "init_lat_text", "text");
    double initLong = QMLDataHelper::getDoubleFromQML(window, "init_long_text", "text");
    double initAlt = QMLDataHelper::getDoubleFromQML(window, "init_alt_text", "text");
    QByteArrayHelper::appendAndStuff(cmd, initLat);
    QByteArrayHelper::appendAndStuff(cmd, initLong);
    QByteArrayHelper::appendAndStuff(cmd, initAlt);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *cmd)
{
    byte typeOfData = (byte)QMLDataHelper::getIntFromQML(window, "typeOfDataComboBox", "currentIndex") + 2;
    byte satelliteIndex = (byte)QMLDataHelper::getIntFromQML(window, "satellite_selection_spinner1", "value");
    QByteArrayHelper::appendAndStuff(cmd, typeOfData);
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void COMHandler::build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *cmd)
{
    byte satelliteIndex = (byte)QMLDataHelper::getIntFromQML(window, "satellites_and_health_spinner", "value");
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void COMHandler::build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *cmd)
{
    bool maskPrimaryPackets = QMLDataHelper::getBoolFromQML(window, "primaryPacketMaskingBit", "checked");
    bool maskSupplPackets = QMLDataHelper::getBoolFromQML(window, "supplPacketMaskingBit", "checked");
    bool maskOtherPackets = QMLDataHelper::getBoolFromQML(window, "otherPacketsMaskingBit", "checked");
    QByteArrayHelper::appendAndStuff(cmd, (byte)(
                       (maskPrimaryPackets ? BIT(0) : 0) |
                       (maskSupplPackets ? BIT(2) : 0) |
                       (maskOtherPackets ? BIT(6) : 0)
                      ));
    QByteArrayHelper::appendAndStuff(cmd, (byte)0);
    QByteArrayHelper::appendAndStuff(cmd, (byte)0);
    QByteArrayHelper::appendAndStuff(cmd, (byte)0);

    qDebug() << *cmd;
}

void COMHandler::build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *cmd)
{
    byte typeOfRequest = (byte)QMLDataHelper::getIntFromQML(window, "timingPacketItem", "timingPacketType");
    QByteArrayHelper::appendAndStuff(cmd, typeOfRequest);

    qDebug() << *cmd;
}

void COMHandler::send_command(int code, int subcode)
{
    QByteArray cmd;
    //qDebug() << code;
    //qDebug() << subcode;

    cmd.append(DLE);
    // Некоторые пакеты содержат только код команды,
    // либо код команды и ее подкод. Для таких пакетов
    // дополнительный метод не нужен.
    QByteArrayHelper::appendAndStuff(&cmd, (byte)code);
    if (subcode > 0) {
        QByteArrayHelper::appendAndStuff(&cmd, (byte)subcode);
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
    case COMMAND_SUPER:
        // Суперпакетов TSIP много, нужно выбрать нужный метод по подкоду.
        switch((byte)subcode) {
        case CMDSUB_SET_PACKET_BROADCAST_MASK:
            build_CMDSUB_SET_PACKET_BROADCAST_MASK(&cmd);
            break;
        case CMDSUB_REQUEST_PRIMARY_TIMING_PACKET:
        case CMDSUB_REQUEST_SUPPL_TIMING_PACKET:
            // Здесь необходимо добавить лишь код, обозначающий, как прислать этот пакет.
            // Для обоих пакетов коды значат одно и то же, поэтому одного метода достаточно.
            build_CMDSUB_REQUEST_TIMING_PACKET(&cmd);
            break;
        }
    }

    cmd.append(DLE);
    cmd.append(ETX);
    //qDebug() << "Starting to write to com...";
    com->write(cmd.constData(), cmd.length());
    com->waitForBytesWritten(300);
    //qDebug() << "finished writing to com";
}

void COMHandler::run()
{
    (this->*methodToStartThreadWith)();
}