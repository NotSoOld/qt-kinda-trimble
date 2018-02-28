#include "packetparser.h"

PacketParser::PacketParser(QByteArray data)
{
    this->data.clear();
    reportCode = (byte)data[0];
    for (int i = 2; i < data.length() - 1; i++) {
        this->data.append(data[i]);
    }
}

byte PacketParser::reportCode()
{
    return reportCode;
}

byte PacketParser::reportSubcode()
{
    return data[0];
}

QString PacketParser::parse_REPORT_UNPARSABLE()
{
    QString res;
    res.append("Невозможно разобрать то, что Вы отправили.\n"
               "Содержимое отправленного пакета:\n");
    for (int i = 0; i < data.length(); i++) {
        res.append(QString("%0 ").arg((byte)data[i], 1, 16));
    }
    return res;
}

QString PacketParser::parse_RPTSUB_FIRMWARE_VERSION()
{
    QString res;

    //reportStruct = new report_foo;                    // Works! P.S. calloc doesn't work
    //memcpy(reportStruct, &data, sizeof(data));        // still under question
    //reportStruct->bar = 4;
    //reportStruct->sss = "My String with random length";   // also seems legit

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x81):\n");
    res.append(QString("- Major версия прошивки: %0\n").arg((byte)data[3]));
    res.append(QString("- Minor версия прошивки: %0\n").arg((byte)data[4]));
    res.append(QString("- Номер сборки прошивки: %0\n").arg(data[5]));
    res.append(QString("- Дата сборки прошивки: %0/%1/%2\n").arg(data[6]).arg(data[7]).arg(TypesConverter::bytesToUInt16(data, 8)));
    res.append(QString("- ID прошивки: "));
    for (int i = 0; i < data[10]; i++) {
        res.append((char)data[11 + i]);
    }

    QDataStream dataStream(data);
    //RPTSUB_FIRMWARE_VERSION_report = new RPTSUB_FIRMWARE_VERSION_reportStruct;    // It is already allocated in stack

    dataStream >> RPTSUB_FIRMWARE_VERSION_report.reportCode >> RPTSUB_FIRMWARE_VERSION_report.reportSubcode
            >> RPTSUB_FIRMWARE_VERSION_report.reserved >> RPTSUB_FIRMWARE_VERSION_report.majorVersion
            >> RPTSUB_FIRMWARE_VERSION_report.minorVersion >> RPTSUB_FIRMWARE_VERSION_report.buildNumber
            >> RPTSUB_FIRMWARE_VERSION_report.month >> RPTSUB_FIRMWARE_VERSION_report.day
            >> RPTSUB_FIRMWARE_VERSION_report.year >> RPTSUB_FIRMWARE_VERSION_report.productNameLength
            >> RPTSUB_FIRMWARE_VERSION_report.productName;
    // --- or... ---
    memcpy(&RPTSUB_FIRMWARE_VERSION_report, &data, sizeof(data));

    return res;
}

QString PacketParser::parse_RPTSUB_HARDWARE_COMPONENT_INFO()
{
    QString res;

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x83):\n");
    res.append(QString("- Серийный номер платы: %0\n").arg(TypesConverter::bytesToUInt32(data, 2)));
    res.append(QString("- Дата сборки платы: %0/%1/%2, %3 часов\n").arg(data[6]).arg(data[7]).arg(TypesConverter::bytesToUInt16(data, 8)).arg(data[10]));
    res.append(QString("- Код платы, ассоциированный с ID: %0\n").arg(TypesConverter::bytesToUInt16(data, 11)));
    res.append(QString("- ID платы: "));
    for (int i = 0; i < data[13]; i++) {
        res.append((char)data[14 + i]);
    }

    return res;
}

QString PacketParser::parse_REPORT_DOUBLE_XYZ_POS()
{
    if (data.length() != 36) {
        return QString("Пакет REPORT_DOUBLE_XYZ_POS (0x83) имеет неверную длину");
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

QString PacketParser::parse_REPORT_DOUBLE_LLA_POS()
{
    if (data.length() != 36) {
        return QString("Пакет REPORT_DOUBLE_LLA_POS (0x84) имеет неверную длину");
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

QString PacketParser::parse_REPORT_SINGLE_XYZ_POS()
{
    if (data.length() != 16) {
        return QString("Пакет REPORT_SINGLE_XYZ_POS (0x42) имеет неверную длину");
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

QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_XYZ()
{
    if (data.length() != 20) {
        return QString("Пакет REPORT_SINGLE_VELOCITY_FIX_XYZ (0x43) имеет неверную длину");
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

QString PacketParser::parse_REPORT_SOFTWARE_VERSION_INFO()
{
    if (data.length() != 10) {
        return QString("Пакет REPORT_SOFTWARE_VERSION_INFO (0x45) имеет неверную длину");
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

QString PacketParser::parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL()
{
    QString message = "-- Уровни сигналов (пакет 0x47): ";
    // Первый байт - количество присланных пар "номер спутника - уровень сигнала".
    for (byte i = 0; i < data[1]; i++) {
       message.append(QString("Спутник %0: %1").arg(data[i * 5 + 2]).arg(TypesConverter::bytesToSingle(data, i * 5 + 3)));
    }
    return message;
}

QString PacketParser::parse_REPORT_SINGLE_LLA_POS()
{
    if (data.length() != 20) {
        return QString("Пакет REPORT_SINGLE_LLA_POS (0x4A) имеет неверную длину");
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

QString PacketParser::parse_REPORT_REQUEST_IO_OPTIONS()
{
    if (data.length() != 4) {
        return QString("Пакет REPORT_REQUEST_IO_OPTIONS (0x55) имеет неверную длину");
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

QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_ENU()
{
    if (data.length() != 20) {
        return QString("Пакет REPORT_SINGLE_VELOCITY_FIX_ENU (0x56) имеет неверную длину");
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

QString PacketParser::parse_REPORT_GPS_SYSTEM_DATA()
{
    if ((int)data[0] == 3) {
        return QString("На спутнике %0 нет данных (пустой пакет REPORT_GPS_SYSTEM_DATA (0x58))").arg(data[2]);
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

QString PacketParser::parse_REPORT_STATUS_SATELLITE_HEALTH()
{
    if (data.length() != 33) {
        return QString("Пакет REPORT_STATUS_SATELLITE_HEALTH (0x59) имеет неверную длину");
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

QString PacketParser::parse_REPORT_RAW_MEASUREMENT_DATA()
{
    if (data.length() != 25) {
        return QString("Пакет REPORT_RAW_MEASUREMENT_DATA (0x5A) имеет неверную длину");
    }

    QString s = "Получен пакет REPORT_RAW_MEASUREMENT_DATA (0x5A):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg(data[0]));
    s.append(QString("- Длина сэмпла, мс: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- Уровень сигнала: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- Кодовая фаза: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- Допплер: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));
    s.append(QString("- Время измерения, с: %0\n").arg(TypesConverter::bytesToDouble(data, 17)));

    return s;
}

QString PacketParser::parse_REPORT_SATELLITE_TRACKING_STATUS()
{
    if (data.length() != 24) {
        return QString("Пакет REPORT_SATELLITE_TRACKING_STATUS (0x5C) имеет неверную длину");
    }

    QString s = "Получен пакет REPORT_SATELLITE_TRACKING_STATUS (0x5C):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg(data[0]));
    s.append(QString("- Номер канала: %0\n").arg((int)data[1] + 1));
    s.append(QString("- Acquisition flag: %0\n").arg((int)data[2] == 0 ? "never acquired" : ((int)data[2] == 1 ? "acquired" : "re-opened search")));
    s.append(QString("- Ephemeris flag: %0\n").arg((int)data[3] == 0 ? "флаг не установлен" : "good ephemeris"));
    s.append(QString("- Уровень сигнала: %0\n").arg(TypesConverter::bytesToSingle(data, 4)));
    s.append(QString("- Время последнего измерения, с: %0\n").arg(TypesConverter::bytesToSingle(data, 8)));
    s.append(QString("- Угол подъема: %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
    s.append(QString("- Угол азимута: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));
    s.append(QString("- Флаг старого измерения: %0\n").arg((int)data[20] == 0 ? "флаг не установлен" : "старое измерение"));

    QString integer_msec_flag = "";
    if ((int)data[21] == 0) {
        integer_msec_flag = "Don't know msec";
    }
    else if ((int)data[21] == 1) {
        integer_msec_flag = "Known from subframe";
    }
    else if ((int)data[21] == 2) {
        integer_msec_flag = "Verified by bit crossing";
    }
    else if ((int)data[21] == 3) {
        integer_msec_flag = "Verified by good fix";
    }
    else if ((int)data[21] == 4) {
        integer_msec_flag = "Suspect msec error";
    }
    s.append(QString("Integer msec flag: %0\n").arg(integer_msec_flag));

    QString bad_data_flag = "";
    if ((int)data[22] == 0) {
        bad_data_flag = "Флаг не установлен";
    }
    else if ((int)data[22] == 1) {
        bad_data_flag = "Ошибка четности";
    }
    else if ((int)data[22] == 2) {
        bad_data_flag = "Bad ephemeris health";
    }
    s.append(QString("Флаг испорченных данных: %0\n").arg(bad_data_flag));

    s.append(QString("- Флаг сбора данных: %0\n").arg((int)data[23] == 0 ? "флаг не установлен" : "сбор в процессе"));

    return s;
}

QString PacketParser::parse_REPORT_SATELLITE_SELECTION_LIST()
{
    QString s = "Получен пакет REPORT_SATELLITE_SELECTION_LIST (0x6D):\n";
    QString fix_dimension = "";
    // Здесь 224 - маска старших трех битов, 7 - младших трех битов.
    if ((data[0] & (byte)224) >> 5 == 1) {
        fix_dimension = "1D clock fix";
    }
    else if ((data[0] & (byte)224) >> 5 == 3) {
        fix_dimension = "2D fix";
    }
    else if ((data[0] & (byte)224) >> 5 == 4) {
        fix_dimension = "3D fix";
    }
    else if ((data[0] & (byte)224) >> 5 == 5) {
        fix_dimension = "OD clock fix";
    }
    s.append(QString("- Fix dimension: %0\n").arg(fix_dimension));

    s.append(QString("- Fix mode: %0\n").arg((data[0] & BIT(3)) ? "manual" : "auto"));
    s.append(QString("- Количество спутников в fix: %0\n").arg(data[0] & (byte)15));
    s.append(QString("- PDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- HDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- VDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- TDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));
    s.append(QString("Список всех включенных спутников (отрицательное значение означает, \
                    что спутник отвергнут для использования в алгоритме T-RAIM):\n"));
    for (int i = 17; i < 17 + (data[0] & (byte)15); i++) {
        s.append(QString("SV PRN: %0\n").arg(data[i]));
    }

    return s;
}

QString PacketParser::parse_RPTSUB_PRIMARY_TIMING_PACKET()
{/*
    if (data.length() != 17) {
        return QString("Пакет RPTSUB_PRIMARY_TIMING_PACKET (0x8F-AB) имеет неверную длину");
    }
*/
    QString s = "Получен пакет RPTSUB_PRIMARY_TIMING_PACKET (0x8F-AB):\n";
    s.append(QString("- Секунды недели GPS: %0\n").arg(TypesConverter::bytesToUInt32(data, 1)));
    s.append(QString("- Номер недели GPS: %0\n").arg(TypesConverter::bytesToUInt16(data, 5)));
    s.append(QString("- Сдвиг UTC, cек: %0\n").arg(TypesConverter::bytesToSInt16(data, 7)));
    s.append("- Флаги времени:\n");
    s.append(QString("-- Время GPS/UTC: %0\n").arg(data[9] & BIT(0) ? "UTC" : "GPS"));
    s.append(QString("-- PPS GPS/UTC: %0\n").arg(data[9] & BIT(1) ? "UTC" : "GPS"));
    s.append(QString("-- Время %0\n").arg(data[9] & BIT(2) ? "не установлено" : "установлено"));
    s.append(QString("-- Информация UTC: %0\n").arg(data[9] & BIT(3) ? "отсутствует" : "присутствует"));
    s.append(QString("-- Время от %0\n").arg(data[9] & BIT(4) ? "пользователя" : "GPS"));
    s.append(QString("- Дата и время: %0/%1/%2 %3:%4:%5\n").arg(data[13]).arg(data[14]).arg(TypesConverter::bytesToUInt16(data, 15))
            .arg(data[12]).arg(data[11]).arg(data[10]));

    return s;
}

QString PacketParser::parse_RPTSUB_SUPPL_TIMING_PACKET()
{/*
    if (data.length() != 68) {
        return QString("Пакет RPTSUB_SUPPL_TIMING_PACKET (0x8F-AC) имеет неверную длину");
    }
*/
    QString s = "Получен пакет RPTSUB_SUPPL_TIMING_PACKET (0x8F-AC):\n";
    s.append(QString("- Режим приемника: "));
    switch (data[1]) {
        case 0: s.append("автоматический (2D/3D)"); break;
        case 1: s.append("один спутник (время)"); break;
        case 3: s.append("горизонтальный (2D)"); break;
        case 4: s.append("полная позиция (3D)"); break;
        case 7: s.append("переопределенные часы"); break;
        default: s.append("неизвестно");
    }
    s.append(QString("\n- Режим дисциплинирования: "));
    switch (data[2]) {
        case 0: s.append("обычный (закреплен за GPS)"); break;
        case 1: s.append("Включение"); break;
        case 2: s.append("автоудержание"); break;
        case 3: s.append("ручное удержание"); break;
        case 4: s.append("восстановление"); break;
        case 5: s.append("не используется"); break;
        case 6: s.append("дисциплинирование выключено"); break;
        default: s.append("неизвестно");
    }
    s.append(QString("\n- Прогресс самоопроса, %: %0\n").arg((char)data[3] ? QString((char)data[3]) : QString("самоопрос в данный момент не проводится")));
    s.append(QString("- Длительность удержания (текущего или последнего, если удержание было отключено), сек: %0\n").arg(TypesConverter::bytesToUInt32(data, 4)));
    s.append("- Критические сигналы:\n");
    s.append(QString("-- DAC at rail: %0\n").arg(data[8] & BIT(4) ? "ДА" : "нет"));
    s.append("- Не такие критические сигналы:\n");
    s.append(QString("-- DAC near rail: %0\n").arg(data[10] & BIT(0) ? "ДА" : "нет"));
    s.append(QString("-- Малое напряжение на антенне (open): %0\n").arg(data[10] & BIT(1) ? "ДА" : "нет"));
    s.append(QString("-- Слишком большое напряжение на антенне (short): %0\n").arg(data[10] & BIT(2) ? "ДА" : "нет"));
    s.append(QString("-- Нет спутников для слежения: %0\n").arg(data[10] & BIT(3) ? "ДА" : "нет"));
    s.append(QString("-- Нет дисциплинирования: %0\n").arg(data[10] & BIT(4) ? "ДА" : "нет"));
    s.append(QString("-- Самоопрос в процессе: %0\n").arg(data[10] & BIT(5) ? "ДА" : "нет"));
    s.append(QString("-- Нет сохраненной позиции: %0\n").arg(data[10] & BIT(6) ? "ДА" : "нет"));
    s.append(QString("-- Leap second pending: %0\n").arg(data[10] & BIT(7) ? "ДА" : "нет"));
    s.append(QString("-- В тестовом режиме: %0\n").arg(data[11] & BIT(0) ? "ДА" : "нет"));
    s.append(QString("-- Позиция под вопросом: %0\n").arg(data[11] & BIT(1) ? "ДА" : "нет"));
    s.append(QString("-- Неполный альманах: %0\n").arg(data[11] & BIT(3) ? "ДА" : "нет"));
    s.append(QString("-- PPS не генерируется: %0\n").arg(data[11] & BIT(4) ? "ДА" : "нет"));
    s.append(QString("- Статус декодирования GPS: "));
    switch (data[12]) {
        case 0x00: s.append("делает фиксы"); break;
        case 0x01: s.append("не имеет времени GPS"); break;
        case 0x03: s.append("PDOP слишком высок"); break;
        case 0x08: s.append("нет юзабельных sat"); break;
        case 0x09: s.append("только один юзабельный sat"); break;
        case 0x0A: s.append("только два юзабельных sat"); break;
        case 0x0B: s.append("только три юзабельных sat"); break;
        case 0x0C: s.append("выбранный sat неюзабелен"); break;
        case 0x10: s.append("TRAIM отбросил фикс"); break;
        default: s.append("неизвестно");
    }
    s.append(QString("\n- Дисциплинированная активность: "));
    switch (data[13]) {
        case 0: s.append("лок фазы"); break;
        case 1: s.append("разогрев осциллятора"); break;
        case 2: s.append("лок частоты"); break;
        case 3: s.append("установка PPS"); break;
        case 4: s.append("инициализация повторяющего фильтра"); break;
        case 5: s.append("компенсация OCXO (удержание)"); break;
        case 6: s.append("неактивно"); break;
        case 7: s.append("не используется"); break;
        case 8: s.append("режим восстановления"); break;
        case 9: s.append("калибровка/управление напряжением"); break;
        default: s.append("неизвестно");
    }
    s.append(QString("- Сдвиг PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));
    s.append(QString("- Сдвиг клока, ppb: %0\n").arg(TypesConverter::bytesToSingle(data, 20)));
    s.append(QString("- Значение DAC: %0\n").arg(TypesConverter::bytesToUInt32(data, 24)));
    s.append(QString("- Напряжение на DAC: %0V\n").arg(TypesConverter::bytesToSingle(data, 28)));
    s.append(QString("- Температура: %0\n").arg(TypesConverter::bytesToSingle(data, 32)));
    s.append(QString("- Широта: %0\n").arg(TypesConverter::bytesToDouble(data, 36)));
    s.append(QString("- Долгота: %0\n").arg(TypesConverter::bytesToDouble(data, 44)));
    s.append(QString("- Высота: %0\n").arg(TypesConverter::bytesToDouble(data, 52)));
    s.append(QString("- Ошибка квантизации PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 60)));

    QDataStream dataStream(data);
    //RPTSUB_SUPPL_TIMING_PACKET_report = new RPTSUB_SUPPL_TIMING_PACKET_reportStruct;
   // dataStream.setFloatingPointPrecision();
    dataStream >> RPTSUB_SUPPL_TIMING_PACKET_report.reportCode >> RPTSUB_SUPPL_TIMING_PACKET_report.reportSubcode
            >> RPTSUB_SUPPL_TIMING_PACKET_report.receiverMode >> RPTSUB_SUPPL_TIMING_PACKET_report.discipliningMode
            >> RPTSUB_SUPPL_TIMING_PACKET_report.selfSurveyProgress >> RPTSUB_SUPPL_TIMING_PACKET_report.holdoverDuration
            >> RPTSUB_SUPPL_TIMING_PACKET_report.criticalAlarmsBF >> RPTSUB_SUPPL_TIMING_PACKET_report.minorAlarmsBF
            >> RPTSUB_SUPPL_TIMING_PACKET_report.gpsDecodingStatus >> RPTSUB_SUPPL_TIMING_PACKET_report.discipliningActivity
            >> RPTSUB_SUPPL_TIMING_PACKET_report.spare1 >> RPTSUB_SUPPL_TIMING_PACKET_report.spare2
            >> RPTSUB_SUPPL_TIMING_PACKET_report.ppsOffset >> RPTSUB_SUPPL_TIMING_PACKET_report.clockOffset
            >> RPTSUB_SUPPL_TIMING_PACKET_report.dacValue >> RPTSUB_SUPPL_TIMING_PACKET_report.dacVoltage
            >> RPTSUB_SUPPL_TIMING_PACKET_report.temperature >> RPTSUB_SUPPL_TIMING_PACKET_report.latitude
            >> RPTSUB_SUPPL_TIMING_PACKET_report.longitude >> RPTSUB_SUPPL_TIMING_PACKET_report.altitude
            >> RPTSUB_SUPPL_TIMING_PACKET_report.ppsQuantizationError;
    // --- or... ---
   // memcpy(&RPTSUB_SUPPL_TIMING_PACKET_report, &data, sizeof(data));

    /* Also:
     * //constData() used because const is desired; otherwise, prefer data() don't forgetting deep copy issues
    const void* poTemp = (const void*)byteArray.constData();
    const MyStruct* poStruct = static_cast< const MyStruct* >(poTemp);
    */

    //updateInterfaceValues();

    return s;
}

void PacketParser::updateInterfaceValues(QQuickWindow *window)
{
    QObject *qmlObject = window->findChild<QObject *>("temperatureLabel");
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg("temperatureLabel");
        return;
    }
    qmlObject->setProperty("text", QVariant(QString("Температура, С: %0 (обновлено ...)").arg(RPTSUB_SUPPL_TIMING_PACKET_report.temperature)));
}