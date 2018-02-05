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
    if (b == DLE) {
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
                     -- Позиция XYZ: (%0; %1; %2)\n \
                     -- Clock bias: %3\n-- Time of fix: %4")
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
                     -- Позиция LLA: (%0 rad; %1 rad; %2 meters)\n \
                     -- Clock bias: %3\n-- Time of fix: %4")
             .arg(latitude).arg(longitude).arg(altitude).arg(clockBias).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_SINGLE_XYZ_POS(QByteArray data)
{
    if (data.length() != 16) {
        qDebug() << "Пакет REPORT_SINGLE_XYZ_POS (0x42) имеет неверную длину";
        return "";
    }

    float X = TypesConverter::bytesToSingle(data, 0);
    float Y = TypesConverter::bytesToSingle(data, 4);
    float Z = TypesConverter::bytesToSingle(data, 8);
    float timeOfFix = TypesConverter::bytesToSingle(data, 12);
    return QString("Получен пакет REPORT_SINGLE_XYZ_POS (0x42):\n \
                     -- Позиция XYZ: (%0; %1; %2)\n \
                     -- Time of fix: %3")
            .arg(X).arg(Y).arg(Z).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_SINGLE_VELOCITY_FIX_XYZ(QByteArray data)
{
    if (data.length() != 20) {
        qDebug() << "Пакет REPORT_SINGLE_VELOCITY_FIX_XYZ (0x43) имеет неверную длину";
        return "";
    }

    float X_vel = TypesConverter::bytesToSingle(data, 0);
    float Y_vel = TypesConverter::bytesToSingle(data, 4);
    float Z_vel = TypesConverter::bytesToSingle(data, 8);
    float biasRate = TypesConverter::bytesToSingle(data, 12);
    float timeOfFix = TypesConverter::bytesToSingle(data, 16);
    return QString("Получен пакет REPORT_SINGLE_VELOCITY_FIX_XYZ (0x43):\n \
                     -- Скорость XYZ (м/с): (%0; %1; %2)\n \
                     -- Bias rate: %3\n-- Time of fix: %4")
            .arg(X_vel).arg(Y_vel).arg(Z_vel).arg(biasRate).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_SOFTWARE_VERSION_INFO(QByteArray data)
{
    if (data.length() != 10) {
        qDebug() << "Пакет REPORT_SOFTWARE_VERSION_INFO (0x45) имеет неверную длину";
        return "";
    }

    return QString("Получен пакет REPORT_SOFTWARE_VERSION_INFO (0x45):\n \
                   ---- Данные с уровня приложения:\n \
                   -- Major версия приложения: %0\n \
                   -- Minor версия приложения: %1\n \
                   -- Дата: %2/%3/%4\n \
                   ---- Данные с уровня ядра:\n \
                   -- Major версия ядра GPS: %5\n \
                   -- Minor версия ядра GPS: %6\n \
                   -- Дата: %7/%8/%9\n")
            .arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[4])
            .arg(data[5]).arg(data[6]).arg(data[7]).arg(data[8]).arg(data[9]);
}

QString COMHandler::parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL(QByteArray data)
{
    QString message = "Уровни сигналов: ";
    // Первый байт - количество присланных пар "номер спутника - уровень сигнала".
    for (byte i = 0; i < data[0]; i++) {
       message.append("Спутник %0: %1").arg(data[i * 5 + 1]).arg(TypesConverter::bytesToSingle(data, i * 5 + 2));
    }
    return message;
}

QString COMHandler::parse_REPORT_SINGLE_LLA_POS(QByteArray data)
{
    if (data.length() != 20) {
        qDebug() << "Пакет REPORT_SINGLE_LLA_POS (0x4A) имеет неверную длину";
        return "";
    }

    float latitude = TypesConverter::bytesToSingle(data, 0);
    float longitude = TypesConverter::bytesToSingle(data, 4);
    float altitude = TypesConverter::bytesToSingle(data, 8);
    float clockBias = TypesConverter::bytesToSingle(data, 12);
    float timeOfFix = TypesConverter::bytesToSingle(data, 16);
    return QString("Получен пакет REPORT_SINGLE_LLA_POS (0x84):\n \
                     -- Позиция LLA: (%0 rad; %1 rad; %2 meters)\n \
                     -- Clock bias: %3\n-- Time of fix: %4")
             .arg(latitude).arg(longitude).arg(altitude).arg(clockBias).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_REQUEST_IO_OPTIONS(QByteArray data)
{
    if (data.length() != 4) {
        qDebug() << "Пакет REPORT_REQUEST_IO_OPTIONS (0x55) имеет неверную длину";
        return "";
    }

    // Данные о позиционировании.
    QString ECEF_on = (data[0] & BIT(0)) ? "вкл" : "выкл";
    QString LLA_on = (data[0] & BIT(1)) ? "вкл" : "выкл";
    QString HAE_or_MSL = (data[0] & BIT(2)) ?  "MSL геоид" : "HAE";
    QString precision = (data[0] & BIT(4)) ? "double" : "single";
    // Данные о скорости.
    QString ECEF_vel_on = (data[1] & BIT(0)) ? "вкл" : "выкл";
    QString ENU_vel_on = (data[1] & BIT(1)) ? "вкл" : "выкл";
    // Данные о времени.
    QString timing = (data[2] & BIT(0)) ? "UTC" : "GPS";
    // Разное.
    QString packet_5A = (data[3] & BIT(0)) ? "вкл" : "выкл";
    QString output_type = (data[3] & BIT(3)) ? "dB-Hz" : "AMU";
    return QString("Получен пакет REPORT_REQUEST_IO_OPTIONS (0x55):\n \
                    -- Данные о позиционировании:\n \
                    ---- ECEF: %0\n ---- LLA: %1\n \
                    ---- HAE или MSL геоид: %2 ---- Точность: %3\n \
                    -- Данные о скорости:\n \
                    ---- ECEF: %4\n ---- ENU: %5\n \
                    -- Тайминг: %6\n-- Разное:\n \
                    ---- Пакет 5А: %7\n ---- Единицы вывода: %8\n")
                   .arg(ECEF_on).arg(LLA_on).arg(HAE_or_MSL).arg(precision)
                   .arg(ECEF_vel_on).arg(ENU_vel_on).arg(timing).arg(packet_5A).arg(output_type);
}

QString COMHandler::parse_REPORT_SINGLE_VELOCITY_FIX_ENU(QByteArray data)
{
    if (data.length() != 20) {
        qDebug() << "Пакет REPORT_SINGLE_VELOCITY_FIX_ENU (0x56) имеет неверную длину";
        return "";
    }

    float east_vel = TypesConverter::bytesToSingle(data, 0);
    float north_vel = TypesConverter::bytesToSingle(data, 4);
    float up_vel = TypesConverter::bytesToSingle(data, 8);
    float biasRate = TypesConverter::bytesToSingle(data, 12);
    float timeOfFix = TypesConverter::bytesToSingle(data, 16);
    return QString("Получен пакет REPORT_SINGLE_VELOCITY_FIX_ENU (0x56):\n \
                     -- Скорость ENU (м/с): (%0; %1; %2)\n \
                     -- Bias rate: %3\n-- Time of fix: %4")
            .arg(east_vel).arg(north_vel).arg(up_vel).arg(biasRate).arg(timeOfFix);
}

QString COMHandler::parse_REPORT_GPS_SYSTEM_DATA(QByteArray data)
{
    if ((int)data[0] == 3) {
        return QString("На спутнике %0 нет данных (пустой пакет REPORT_GPS_SYSTEM_DATA (0x58)).").arg(data[2]);
    }

    QString s = "Получен пакет REPORT_GPS_SYSTEM_DATA (0x58):\n";
    switch (data[1]) {
        case 2:     // Альманах
            s.append("Данные альманаха:\n");
            s.append(QString("T_oa (raw): %0\n").arg(data[4]));
            s.append(QString("SV_HEALTH: %0\n").arg(data[5]));
            s.append(QString("e: %0\n").arg(TypesConverter::bytesToSingle(data, 6)));
            s.append(QString("t_oa: %0\n").arg(TypesConverter::bytesToSingle(data, 10)));
            s.append(QString("i_o: %0\n").arg(TypesConverter::bytesToSingle(data, 14)));
            s.append(QString("OMEGADOT: %0\n").arg(TypesConverter::bytesToSingle(data, 18)));
            s.append(QString("sqrt(A): %0\n").arg(TypesConverter::bytesToSingle(data, 22)));
            s.append(QString("(OMEGA)0: %0\n").arg(TypesConverter::bytesToSingle(data, 26)));
            s.append(QString("(OMEGA): %0\n").arg(TypesConverter::bytesToSingle(data, 30)));
            s.append(QString("M_0: %0\n").arg(TypesConverter::bytesToSingle(data, 34)));
            s.append(QString("a_f0: %0\n").arg(TypesConverter::bytesToSingle(data, 38)));
            s.append(QString("a_f1: %0\n").arg(TypesConverter::bytesToSingle(data, 42)));
            s.append(QString("Axis: %0\n").arg(TypesConverter::bytesToSingle(data, 46)));
            s.append(QString("n: %0\n").arg(TypesConverter::bytesToSingle(data, 50)));
            s.append(QString("OMEGA_n: %0\n").arg(TypesConverter::bytesToSingle(data, 54)));
            s.append(QString("ODOT_n: %0\n").arg(TypesConverter::bytesToSingle(data, 58)));
            s.append(QString("t_zc: %0\n").arg(TypesConverter::bytesToSingle(data, 62)));
            s.append(QString("Номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 66)));
            s.append(QString("WNa: %0\n").arg(TypesConverter::bytesToUInt16(data, 68)));
            break;
        case 3:     // Здоровье
            s.append("Данные о здоровье:\n");
            s.append(QString("Номер недели для здоровья: %0\n").arg(data[4]));
            for (int i = 5; i <= 36; i++) {
                s.append(QString("Здоровье спутника %0: %1\n").arg(data[i])).arg(i - 4);
            }
            s.append(QString("t_oa для здоровья: %0\n").arg(data[37]));
            s.append(QString("Текущее t_oa: %0\n").arg(data[38]));
            s.append(QString("Текущий номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            break;
        case 4:     // Ионосфера
            s.append("Данные об ионосфере:\n");
            s.append(QString("alpha0 = %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
            s.append(QString("alpha1 = %0\n").arg(TypesConverter::bytesToSingle(data, 16)));
            s.append(QString("alpha2 = %0\n").arg(TypesConverter::bytesToSingle(data, 20)));
            s.append(QString("alpha3 = %0\n").arg(TypesConverter::bytesToSingle(data, 24)));
            s.append(QString("beta0 = %0\n").arg(TypesConverter::bytesToSingle(data, 28)));
            s.append(QString("beta1 = %0\n").arg(TypesConverter::bytesToSingle(data, 32)));
            s.append(QString("beta2 = %0\n").arg(TypesConverter::bytesToSingle(data, 36)));
            s.append(QString("beta3 = %0\n").arg(TypesConverter::bytesToSingle(data, 40)));
            break;
        case 5:     // Информация UTC
            s.append("Информация UTC:\n");
            s.append(QString("A0 = %0\n").arg(TypesConverter::bytesToDouble(data, 17)));
            s.append(QString("A1 = %0\n").arg(TypesConverter::bytesToSingle(data, 25)));
            s.append(QString("delta t_LS = %0\n").arg(TypesConverter::bytesToSInt16(data, 29)));
            s.append(QString("t_ot = %0\n").arg(TypesConverter::bytesToSingle(data, 31)));
            s.append(QString("WN_t = %0\n").arg(TypesConverter::bytesToUInt16(data, 35)));
            s.append(QString("WN_LSF = %0\n").arg(TypesConverter::bytesToUInt16(data, 37)));
            s.append(QString("DN = %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            s.append(QString("delta t_LSf = %0\n").arg(TypesConverter::bytesToSInt16(data, 41)));
            break;
        case 6:     // Информация ephemeris
            s.append("Ephemeris info:\n");
            s.append(QString("Номер спутника (SV PRN): %0\n").arg(data[4]));
            s.append(QString("t_ephem = %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
            s.append(QString("Номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 9)));
            s.append(QString("Точность спутника (raw): %0\n").arg(data[13]));
            s.append(QString("Здоровье спутника: %0\n").arg(data[14]));
            s.append(QString("Issue of data clock: %0\n").arg(TypesConverter::bytesToUInt16(data, 15)));
            s.append(QString("t_GD = %0\n").arg(TypesConverter::bytesToSingle(data, 17)));
            s.append(QString("t_oc = %0\n").arg(TypesConverter::bytesToSingle(data, 21)));
            s.append(QString("a_f2 = %0\n").arg(TypesConverter::bytesToSingle(data, 25)));
            s.append(QString("a_f1 = %0\n").arg(TypesConverter::bytesToSingle(data, 29)));
            s.append(QString("a_f0 = %0\n").arg(TypesConverter::bytesToSingle(data, 33)));
            s.append(QString("Точность спутника: %0\n").arg(TypesConverter::bytesToSingle(data, 37)));
            s.append(QString("Issue of data ephemeris: %0\n").arg(data[41]));
            s.append(QString("C_rs = %0\n").arg(TypesConverter::bytesToSingle(data, 43)));
            s.append(QString("delta n = %0\n").arg(TypesConverter::bytesToSingle(data, 47)));
            s.append(QString("M0 = %0\n").arg(TypesConverter::bytesToDouble(data, 51)));
            s.append(QString("C_uc = %0\n").arg(TypesConverter::bytesToSingle(data, 59)));
            s.append(QString("e = %0\n").arg(TypesConverter::bytesToDouble(data, 63)));
            s.append(QString("C_US = %0\n").arg(TypesConverter::bytesToSingle(data, 71)));
            s.append(QString("sqrt(A) = %0\n").arg(TypesConverter::bytesToDouble(data, 75)));
            s.append(QString("t_oe = %0\n").arg(TypesConverter::bytesToSingle(data, 83)));
            s.append(QString("C_ic = %0\n").arg(TypesConverter::bytesToSingle(data, 87)));
            s.append(QString("(OMEGA)0 = %0\n").arg(TypesConverter::bytesToDouble(data, 91)));
            s.append(QString("C_is = %0\n").arg(TypesConverter::bytesToSingle(data, 99)));
            s.append(QString("i_o = %0\n").arg(TypesConverter::bytesToDouble(data, 103)));
            s.append(QString("C_rc = %0\n").arg(TypesConverter::bytesToSingle(data, 111)));
            s.append(QString("(OMEGA) = %0\n").arg(TypesConverter::bytesToDouble(data, 115)));
            s.append(QString("OMEGADOT = %0\n").arg(TypesConverter::bytesToSingle(data, 123)));
            s.append(QString("IDOT = %0\n").arg(TypesConverter::bytesToSingle(data, 127)));
            s.append(QString("Axis = %0\n").arg(TypesConverter::bytesToDouble(data, 131)));
            s.append(QString("n = %0\n").arg(TypesConverter::bytesToDouble(data, 139)));
            s.append(QString("r1me2 = %0\n").arg(TypesConverter::bytesToDouble(data, 147)));
            s.append(QString("OMEGA_n = %0\n").arg(TypesConverter::bytesToDouble(data, 155)));
            s.append(QString("ODOT_n = %0\n").arg(TypesConverter::bytesToDouble(data, 163)));
            break;
    }

    return s;
}

QString COMHandler::parse_REPORT_STATUS_SATELLITE_HEALTH(QByteArray data)
{
    if (data.length() != 33) {
        qDebug() << "Пакет REPORT_STATUS_SATELLITE_HEALTH (0x59) имеет неверную длину";
        return "";
    }

    QString s = "";
    if ((int)data[0] == 3) {
        // Пришла информация о включении спутников.
        s.append("Информация о включении спутников:\n");
        for (int i = 1; i <= 32; i++) {
            s.append(QString("- Спутник %0: %1\n").arg(i).arg(data[i] ? "включен" : "выключен"));
        }
    }
    else {
        // Пришла информация о принятии здоровь спутников во внимание.
        s.append("Информация о принятии здоровья спутников во внимание:\n");
        for (int i = 1; i <= 32; i++) {
            s.append(QString("- Спутник %0: %1\n").arg(i).arg(data[i] ? "принимается" : "игнорируется"));
        }
    }

    return s;
}

QString COMHandler::parse_REPORT_RAW_MEASUREMENT_DATA(QByteArray data)
{
    if (data.length() != 25) {
        qDebug() << "Пакет REPORT_RAW_MEASUREMENT_DATA (0x5A) имеет неверную длину";
        return "";
    }

    QString s = "Пакет REPORT_RAW_MEASUREMENT_DATA (0x5A):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg(data[0]);
    s.append(QString("- Длина сэмпла, мс: %0\n").arg(TypesConverter::bytesToSingle(data, 1));
    s.append(QString("- Уровень сигнала: %0\n").arg(TypesConverter::bytesToSingle(data, 5));
    s.append(QString("- Кодовая фаза: %0\n").arg(TypesConverter::bytesToSingle(data, 9));
    s.append(QString("- Допплер: %0\n").arg(TypesConverter::bytesToSingle(data, 13));
    s.append(QString("- Время измерения, с: %0\n").arg(TypesConverter::bytesToDouble(data, 17));

    return s;
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
        case REPORT_SINGLE_XYZ_POS:
            message.append(parse_REPORT_SINGLE_XYZ_POS(data));
            break;
        case REPORT_SINGLE_VELOCITY_FIX_XYZ:
            message.append(parse_REPORT_SINGLE_VELOCITY_FIX_XYZ(data));
            break;
        case REPORT_SOFTWARE_VERSION_INFO:
            message.append(parse_REPORT_SOFTWARE_VERSION_INFO(data));
            break;
        case REPORT_TRACKED_SATELLITES_SINGAL_LVL:
            message.append(parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL(data));
            break;
        case REPORT_SINGLE_LLA_POS:
            message.append(parse_REPORT_SINGLE_LLA_POS(data));
            break;
        case REPORT_REQUEST_IO_OPTIONS:
            message.append(parse_REPORT_REQUEST_IO_OPTIONS(data));
            break;
        case REPORT_SINGLE_VELOCITY_FIX_ENU:
            message.append(parse_REPORT_SINGLE_VELOCITY_FIX_ENU(data));
            break;
        case REPORT_GPS_SYSTEM_DATA:
            message.append(parse_REPORT_GPS_SYSTEM_DATA(data));
            break;
        case REPORT_STATUS_SATELLITE_HEALTH:
            message.append(parse_REPORT_STATUS_SATELLITE_HEALTH(data));
            break;
        case REPORT_RAW_MEASUREMENT_DATA:
            message.append(parse_REPORT_RAW_MEASUREMENT_DATA(data));
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

void COMHandler::build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *cmd)
{
    byte satelliteIndex = (byte)getIntFromQML("satellites_and_health_spinner", "value");
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
    // либо код команды и ее подкод. Для таких пакетов
    // дополнительный метод не нужен.
    append(&cmd, (byte)code);
    if (subcode > 0) {
        append(&cmd, (byte)subcode);
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
