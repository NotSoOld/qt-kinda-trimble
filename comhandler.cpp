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
    qDebug() << "COM is configured";
    name = portName;
}

void COMHandler::finishCOM()
{
    com->flush();
    com->close();
}

QByteArray COMHandler::toByteArray(double d)
{
    __double *result = (__double *)calloc(1, sizeof(__double));
    result->value = d;
    QByteArray res;
    for (int i = 0; i < 8; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

QByteArray COMHandler::toByteArray(float f)
{
    __single *result = (__single *)calloc(1, sizeof(__single));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 4; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

QByteArray COMHandler::toByteArray(unsigned short f)
{
    _int16 *result = (_int16 *)calloc(1, sizeof(_int16));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 2; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

QByteArray COMHandler::toByteArray(unsigned int f)
{
    _int32 *result = (_int32 *)calloc(1, sizeof(_int32));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 4; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

void COMHandler::append(QByteArray *bytes, double d)
{
    QByteArray doubleBytes = toByteArray(d);
    for (int i = 0; i < 8; i++) {
        append(bytes, (byte)doubleBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, float f)
{
    QByteArray floatBytes = toByteArray(f);
    for (int i = 0; i < 4; i++) {
        append(bytes, (byte)floatBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, unsigned short f)
{
    QByteArray shortBytes = toByteArray(f);
    for (int i = 0; i < 2; i++) {
        append(bytes, (byte)shortBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, unsigned int f)
{
    QByteArray intBytes = toByteArray(f);
    for (int i = 0; i < 4; i++) {
        append(bytes, (byte)intBytes[i]);
    }
}

void COMHandler::append(QByteArray *bytes, byte b)
{
    //qDebug() << "bytes version called";
    bytes->append(b);
    // Perform stuffing for DLE bytes.
    if (b == DLE)
    {
        bytes->append(DLE);
    }
}

void COMHandler::send_COMMAND_REQUEST_STATUS_AND_POS()
{
    QByteArray cmd;
    cmd.append(DLE);
    append(&cmd, (byte)COMMAND_REQUEST_STATUS_AND_POS);
    cmd.append(DLE);
    cmd.append(ETX);
    com->write(cmd.constData(), cmd.length());
    com->waitForBytesWritten(1000);
    qDebug() << "send_COMMAND_REQUEST_STATUS_AND_POS";
}

unsigned int COMHandler::bytesToInt32(QByteArray bytes, int start)
{
    _int32 *result = (_int32 *)calloc(1, sizeof(_int32));
    for (int i = start; i < start + 4; i++) {
        result->bytes[i - start] = bytes[i];
    }
    unsigned int res = result->value;
    free(result);
    return res;
}

unsigned short COMHandler::bytesToInt16(QByteArray bytes, int start)
{
    _int16 *result = (_int16 *)calloc(1, sizeof(_int16));
    for (int i = start; i < start + 2; i++) {
        result->bytes[i - start] = bytes[i];
    }
    unsigned short res = result->value;
    free(result);
    return res;
}

float COMHandler::bytesToSingle(QByteArray bytes, int start)
{
    __single *result = (__single *)calloc(1, sizeof(__single));
    for (int i = start; i < start + 4; i++) {
        result->bytes[i - start] = bytes[i];
    }
    float res = result->value;
    free(result);
    return res;
}

double COMHandler::bytesToDouble(QByteArray bytes, int start)
{
    __double *result = (__double *)calloc(1, sizeof(__double));
    for (int i = start; i < start + 8; i++) {
        result->bytes[i - start] = bytes[i];
    }
    double res = result->value;
    free(result);
    return res;
}

QString COMHandler::parseDoubleXYZPos(QByteArray data)
{
    if (data.length() != 36) {
        qDebug() << "Probably corrupted REPORT_DOUBLE_XYZ_POS (0x83) packet";
    }

    double X = bytesToDouble(data, 0);
    double Y = bytesToDouble(data, 8);
    double Z = bytesToDouble(data, 16);
    double clockBias = bytesToDouble(data, 24);
    float timeOfFix = bytesToSingle(data, 32);
    return QString("Received REPORT_DOUBLE_XYZ_POS (0x83) packet:\n \
                     --Position XYZ: (%0; %1; %2)\n \
                     --Clock bias: %3\n--Time of fix: %4")
            .arg(X).arg(Y).arg(Z).arg(clockBias).arg(timeOfFix);
}

QString COMHandler::parseDoubleLLAPos(QByteArray data)
{
    if (data.length() != 36) {
        qDebug() << "Probably corrupted REPORT_DOUBLE_LLA_POS (0x84) packet";
    }

    double latitude = bytesToDouble(data, 0);
    double longitude = bytesToDouble(data, 8);
    double altitude = bytesToDouble(data, 16);
    double clockBias = bytesToDouble(data, 24);
    float timeOfFix = bytesToSingle(data, 32);
    return QString("Received REPORT_DOUBLE_LLA_POS (0x84) packet:\n \
                     --Position LLA: (%0 rad; %1 rad; %2 meters)\n \
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
            // Parse error!
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
        // Here we have reportCode and our data in array from message.
        QString message;
        switch ((byte)reportCode) {
        case REPORT_DOUBLE_XYZ_POS:
            message.append(parseDoubleXYZPos(data));
            break;
        case REPORT_DOUBLE_LLA_POS:
            message.append(parseDoubleLLAPos(data));
            break;
        default:
            message = QString("(Yet) unknown command 0x%0").arg((byte)reportCode, 1, 16);
            break;
        }
        emit appendReceivedText(message);

        parser = ParserStatus::Waiting_for_message;
    }
}

void COMHandler::send_COMMAND_SET_IO_OPTIONS()
{
    QByteArray cmd;
    QObject *ecefCheck = window->findChild<QObject *>("eCEFcheck");
    if (!ecefCheck) {
        qDebug() << "could not find ecefCheck";
    }
    bool ecefChecked = ecefCheck->property("checked").toBool();
    QObject *llaCheck = window->findChild<QObject *>("lLAcheck");
    if (!llaCheck) {
        qDebug() << "could not find llaCheck";
    }
    bool llaChecked = llaCheck->property("checked").toBool();
    QObject *precisionRB = window->findChild<QObject *>("doublePrecRB");
    if (!precisionRB) {
        qDebug() << "could not find precisionRB";
    }
    bool precision = precisionRB->property("checked").toBool();
    QObject *gpsTimeRB = window->findChild<QObject *>("gpsTimeRB");
    if (!gpsTimeRB) {
        qDebug() << "could not find gpsTimeRB";
    }
    bool gpsTime = gpsTimeRB->property("checked").toBool();

    cmd.append(DLE);
    cmd.append(COMMAND_SET_IO_OPTIONS);
    cmd.append((ecefChecked ? BIT(0) : 0) |
               (llaChecked ? BIT(1) : 0) |
               (precision ? BIT(4) : 0));
    cmd.append((char)0);
    cmd.append(gpsTime ? 0 : BIT(0));
    cmd.append((char)0);
    cmd.append(DLE);
    cmd.append(ETX);
    qDebug() << cmd;
    com->write(cmd.constData(), cmd.length());
    com->waitForBytesWritten(1000);
}

void COMHandler::run()
{
    (this->*methodToStartThreadWith)();
}
