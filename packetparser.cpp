#include "packetparser.h"

PacketParser::PacketParser(QByteArray data)
{
    this->data = data;
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
    QString message = "Уровни сигналов: ";
    // Первый байт - количество присланных пар "номер спутника - уровень сигнала".
    for (byte i = 0; i < data[0]; i++) {
       message.append("Спутник %0: %1").arg(data[i * 5 + 1]).arg(TypesConverter::bytesToSingle(data, i * 5 + 2));
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
    s.append(QString("Fix dimension: %0\n").arg(fix_dimension));

    s.append(QString("Fix mode: %0").arg((data[0] & BIT(3)) ? "manual" : "auto"));
    s.append(QString("Количество спутников в fix: %0").arg(data[0] & (byte)7));
    s.append(QString("- PDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- HDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- VDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- TDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));
    s.append(QString("Список всех включенных спутников (отрицательное значение означает, \
                    что спутник отвергнут для использования в алгоритме T-RAIM):\n"));
    for (int i = 17; i < 17 + (data[0] & (byte)7); i++) {
        s.append(QString("SV PRN: %0\n").arg(data[i]));
    }

    return s;
}
