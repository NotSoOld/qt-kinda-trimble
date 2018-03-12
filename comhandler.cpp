#include "comhandler.h"

QSerialPort *COMHandler::com;
QString COMHandler::name;
QByteArray COMHandler::readedData;
byte COMHandler::previouslyReadedChar;
QList<QSerialPortInfo> COMHandler::portsList;

void COMHandler::getSerialPortsList()
{
    // Перед обновлением списка доступных портов (а это происходит
    // только при открытии/переоткрытии линии) нужно закрыть старое
    //соединение, чтобы оно отображалось в списке доступных.
    finishCOM();

    portsList = QSerialPortInfo::availablePorts();
    int openPortsNumber = portsList.length();
    for (int i = 0; i < openPortsNumber; i++) {
        /*
        qDebug() << portsList[i].portName();
        qDebug() << portsList[i].serialNumber();
        qDebug() << portsList[i].description();
        qDebug() << portsList[i].manufacturer();
        qDebug() << portsList[i].systemLocation();
        */
        if (portsList[i].isBusy()) {
            portsList.removeAt(i);
            i--;
            openPortsNumber--;
        }
    }

    QVariantList portNamesList;
    for (int i = 0; i < openPortsNumber; i++) {
        portNamesList.append(QVariant(QString("%0 (%1)").arg(portsList[i].description()).arg(portsList[i].portName())));
    }

    QObject *portsComboBox = QMLDataHelper::mainWindow->findChild<QObject *>("com_init_window");
    portsComboBox = portsComboBox->findChild<QObject *>("portsComboBox");
    portsComboBox->setProperty("model", portNamesList);
    portsComboBox->setProperty("currentIndex", 0);
}

void COMHandler::configureCOM(int portIndex, int baudRate, int dataBits, int parity, int flowControl, int stopBits)
{
    // Если порт уже был открыт, его сначала нужно закрыть.
    // Однако он не может оказаться в списке, если он уже открыт, так что...
    //finishCOM();

    QString portName = portsList[portIndex].portName();
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

    emit appendReceivedText(QString("VirtualCOM %0 (%1) подключен")
                            .arg(portsList[portIndex].description()).arg(portName));
}

void COMHandler::finishCOM()
{
    if (com == nullptr)
        return;
    if (!(com->isOpen()))
        return;

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
        if (previouslyReadedChar == DLE && (quint8)readedChar == ETX) {
            // Обнаружен конец пакета. ETX также отбрасываем.
            //qDebug() << readedData;
            receiveReport();
            // После вызова метода выше всё содержимое readedData было разобрано
            // (оно содержало полный пакет), поэтому теперь можно его очистить.
            readedData.clear();
           // qDebug() << "cleared readed data";

        }

        else if (!(previouslyReadedChar == DLE && (quint8)readedChar == DLE)) {
            // Если обнаружено экранирование байта DLE, не нужно заносить его в полученные данные.
            readedData.append(readedChar);
        }

        previouslyReadedChar = (quint8)readedChar;
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
        message.append(parser->parse_REPORT_DOUBLE_XYZ_POS(this));
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
    case REPORT_LAST_FIX_INFO:
        message.append(parser->parse_REPORT_LAST_FIX_INFO());
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
            //parser->updateInterfaceValues();
            break;
        case RPTSUB_PACKET_BROADCAST_MASK:
            message.append(parser->parse_RPTSUB_PACKET_BROADCAST_MASK());
            break;
        default:
            message.append(QString("Это точно был пакет REPORT_SUPER (0х8F)? Подкод %0 не распознан.")
                           .arg(parser->reportSubcode(), 1, 16));
        }
        break;
    default:
        message = QString("Неизвестный пакет 0x%0 ЛИБО проблемы с пониманием. Пакет отброшен")
                .arg(parser->reportCode(), 1, 16);
    }
    emit appendReceivedText(message);
}




void COMHandler::send_command(int code, int subcode)
{
    QByteArray cmd;
    CommandBuilder *cmdBuilder = new CommandBuilder();
    qDebug() << code;
    qDebug() << subcode;

    cmd.append(DLE);
    // Некоторые пакеты содержат только код команды, либо код команды и ее подкод.
    // Для таких пакетов дополнительный метод не нужен.
    QByteArrayHelper::appendAndStuff(&cmd, (quint8)code);
    if (subcode > 0) {
        QByteArrayHelper::appendAndStuff(&cmd, (quint8)subcode);
    }
    // Для команд, которые содержат дополнительную информацию,
    // выбирается соответствующий метод.
    switch ((quint8)code) {
    case COMMAND_SET_IO_OPTIONS:
        // (Если == -1, то это запрос настроек, а не их установка, и формирование пакета не требуется.)
        if (subcode == 0) {
            cmdBuilder->build_COMMAND_SET_IO_OPTIONS(&cmd);
        }
        break;
    case COMMAND_SATELLITE_SELECTION:
    case COMMAND_REQUEST_LAST_RAW_MEASUREMENT:
    case COMMAND_REQUEST_SATELLITE_TRACKING_STATUS:
        // Индекс спутника для этих команд приходит в subcode. При этом
        // подкод == 0 не добавится выше, поэтому это нужно сделать отдельно.
        if (subcode == 0) {
            cmd.append((quint8)subcode);
        }
        break;
    case COMMAND_ACCURATE_INIT_POS_XYZ:
        cmdBuilder->build_COMMAND_ACCURATE_INIT_POS_XYZ(&cmd);
        break;
    case COMMAND_ACCURATE_INIT_POS_LLA:
        cmdBuilder->build_COMMAND_ACCURATE_INIT_POS_LLA(&cmd);
        break;
    case COMMAND_REQUEST_SATELLITE_SYSTEM_DATA:
        // Подкод здесь всегда == 1, он же - первый информационный байт (см. документацию).
        cmdBuilder->build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(&cmd);
        break;
    case COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH:
        // Для всех возможных операций (1 - 6) номер операции уже пришел в подкоде.
        // Для всех операций также нужно добавить номер спутника, которого эта операция касается
        // (для операций 3 и 6 номер спутника не важен, однако пакет всё равно нужно дополнить).
        cmdBuilder->build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(&cmd);
        break;
    case COMMAND_SUPER:
        // Суперпакетов TSIP много, нужно выбрать нужный метод по подкоду.
        switch((quint8)subcode) {
        case CMDSUB_SET_PACKET_BROADCAST_MASK:
            cmdBuilder->build_CMDSUB_SET_PACKET_BROADCAST_MASK(&cmd);
            break;
        case CMDSUB_REQUEST_PRIMARY_TIMING_PACKET:
        case CMDSUB_REQUEST_SUPPL_TIMING_PACKET:
            // Здесь необходимо добавить лишь код, обозначающий, как прислать этот пакет.
            // Для обоих пакетов коды значат одно и то же, поэтому одного метода достаточно.
            cmdBuilder->build_CMDSUB_REQUEST_TIMING_PACKET(&cmd);
            break;
        }
    }

    qDebug() << cmd;
    cmd.append(DLE);
    cmd.append(ETX);
    com->write(cmd.constData(), cmd.length());
    com->waitForBytesWritten(300);
}

void COMHandler::run()
{
    (this->*methodToStartThreadWith)();
}