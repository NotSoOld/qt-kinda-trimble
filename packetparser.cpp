#include "packetparser.h"

PacketParser::PacketParser(QByteArray data)
{
    this->data.clear();
    _reportCode = (quint8)data[1];
    for (int i = 2; i < data.length() - 1; i++) {
        this->data.append((quint8)data[i]);
    }
}

byte PacketParser::reportCode()
{
    return _reportCode;
}

byte PacketParser::reportSubcode()
{
    return (quint8)data[0];
}

QString PacketParser::parse_REPORT_UNPARSABLE()
{
    QString res;
    res.append("Невозможно разобрать то, что Вы отправили.\n"
               "Содержимое отправленного пакета:\n");
    res.append(QString("Код (первый байт) отправленного пакета: 0x%0\n").arg((quint8)data[0], 1, 16));
    for (int i = 1; i < data.length(); i++) {
        res.append(QString("0x%0 ").arg((quint8)data[i], 1, 16));
    }
    return res;
}

QString PacketParser::parse_RPTSUB_FIRMWARE_VERSION()
{
    QString res;
    QString info;

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x81):\n");
    info.append(QString("- Major версия прошивки: %0\n").arg((quint8)data[2]));
    info.append(QString("- Minor версия прошивки: %0\n").arg((quint8)data[3]));
    info.append(QString("- Номер сборки прошивки: %0\n").arg((quint8)data[4]));
    info.append(QString("- Дата сборки прошивки: %0/%1/%2\n").arg((quint8)data[5]).arg((quint8)data[6]).arg(TypesConverter::bytesToUInt16(data, 7)));
    info.append(QString("- ID прошивки: "));
    for (quint8 i = 0; i < (quint8)data[9]; i++) {
        info.append((char)data[10 + i]);
    }
    res.append(info);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_RPTSUB_FIRMWARE_VERSION_label");
    qmlObject->setProperty("text", QVariant(info));


/*
    QDataStream dataStream(data);

    dataStream >> RPTSUB_FIRMWARE_VERSION_report.reportSubcode
            >> RPTSUB_FIRMWARE_VERSION_report.reserved >> RPTSUB_FIRMWARE_VERSION_report.majorVersion
            >> RPTSUB_FIRMWARE_VERSION_report.minorVersion >> RPTSUB_FIRMWARE_VERSION_report.buildNumber
            >> RPTSUB_FIRMWARE_VERSION_report.month >> RPTSUB_FIRMWARE_VERSION_report.day
            >> RPTSUB_FIRMWARE_VERSION_report.year >> RPTSUB_FIRMWARE_VERSION_report.productNameLength
            >> RPTSUB_FIRMWARE_VERSION_report.productName;
*/
    return res;
}

QString PacketParser::parse_RPTSUB_HARDWARE_COMPONENT_INFO()
{
    QString res;
    QString info;

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x83):\n");
    info.append(QString("- Серийный номер платы: %0\n").arg(TypesConverter::bytesToUInt32(data, 1)));
    info.append(QString("- Дата сборки платы: %0/%1/%2, %3 часов\n").arg((quint8)data[5]).arg((quint8)data[6]).arg(TypesConverter::bytesToUInt16(data, 7)).arg((quint8)data[9]));
    info.append(QString("- Код платы, ассоциированный с ID: %0\n").arg(TypesConverter::bytesToUInt16(data, 10)));
    info.append(QString("- ID платы: "));
    for (quint8 i = 0; i < (quint8)data[12]; i++) {
        info.append((char)data[13 + i]);
    }
    res.append(info);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_RPTSUB_HARDWARE_COMPONENT_INFO_label");
    qmlObject->setProperty("text", QVariant(info));

    return res;
}

QString PacketParser::parse_REPORT_DOUBLE_XYZ_POS(COMHandler *com)
{
    QString res;
    res.append("Получен пакет REPORT_DOUBLE_XYZ_POS (0x83):\n");
    res.append(QString("- Позиция (Х; Y; Z): (%0 м; %1 м; %2 м)\n").arg(TypesConverter::bytesToDouble(data, 0)).arg(TypesConverter::bytesToDouble(data, 8))
            .arg(TypesConverter::bytesToDouble(data, 16)));
    res.append(QString("- Clock bias: %0\n").arg(TypesConverter::bytesToDouble(data, 24)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToDouble(data, 32)));

    double X = TypesConverter::bytesToDouble(data, 0);
    double Y = TypesConverter::bytesToDouble(data, 8);
    double Z = TypesConverter::bytesToDouble(data, 16);

//    emit com->newValuesGained(QVariant(X), QVariant(Y));

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Х: %0 м").arg(X, 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("yPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Y: %0 м").arg(Y, 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("zPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Z: %0 м").arg(Z, 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(TypesConverter::bytesToSingle(data, 24), 0, 'f', 6)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 4)));

    return res;
}

QString PacketParser::parse_REPORT_DOUBLE_LLA_POS()
{
    QString res;
    res.append("Получен пакет REPORT_DOUBLE_LLA_POS (0x84):\n");
    res.append(QString("- Позиция LLA: (%0 рад; %1 рад; %2 м)\n").arg(TypesConverter::bytesToDouble(data, 0))
               .arg(TypesConverter::bytesToDouble(data, 8)).arg(TypesConverter::bytesToDouble(data, 16)));
    res.append(QString("- Clock bias: %0\n").arg(TypesConverter::bytesToDouble(data, 24)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToDouble(data, 32)));

    double latitude = TypesConverter::bytesToDouble(data, 0) * (180 / PI);
    double longitude = TypesConverter::bytesToDouble(data, 8) * (180 / PI);
    double altitude = TypesConverter::bytesToDouble(data, 16);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 широты").arg(latitude, 0, 'f').arg(latitude > 0 ? "северной" : "южной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 долготы").arg(longitude, 0, 'f').arg(longitude > 0 ? "восточной" : "западной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(altitude, 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(TypesConverter::bytesToSingle(data, 24), 0, 'f', 6)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 4)));

    return res;
}

QString PacketParser::parse_REPORT_SINGLE_XYZ_POS()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_XYZ_POS (0x42):\n");
    res.append(QString("- Позиция (Х; Y; Z): (%0 м; %1 м; %2 м)\n").arg(TypesConverter::bytesToSingle(data, 0))
               .arg(TypesConverter::bytesToSingle(data, 4)).arg(TypesConverter::bytesToSingle(data, 8)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToSingle(data, 12)));

    float X = TypesConverter::bytesToSingle(data, 0);
    float Y = TypesConverter::bytesToSingle(data, 4);
    float Z = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Х: %0 м").arg(X, 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("yPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Y: %0 м").arg(Y, 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("zPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Z: %0 м").arg(Z, 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(TypesConverter::bytesToSingle(data, 8), 0, 'f', 6)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 12), 0, 'f', 4)));

    return res;
}

QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_XYZ()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_VELOCITY_FIX_XYZ (0x43):\n");
    res.append(QString("- Скорость (Х; Y; Z): (%0 м/с; %1 м/с; %2 м/с)\n").arg(TypesConverter::bytesToSingle(data, 0))
               .arg(TypesConverter::bytesToSingle(data, 4)).arg(TypesConverter::bytesToSingle(data, 8)));
    res.append(QString("- Bias rate: %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));

    float X_vel = TypesConverter::bytesToSingle(data, 0);
    float Y_vel = TypesConverter::bytesToSingle(data, 4);
    float Z_vel = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("Скорость по Х: %0 м/с").arg(X_vel, 0, 'f', 3)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("yVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("Скорость по Y: %0 м/с").arg(Y_vel, 0, 'f', 3)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("zVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("Скорость по Z: %0 м/с").arg(Z_vel, 0, 'f', 3)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzVelocityBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м/с").arg(TypesConverter::bytesToSingle(data, 12), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzVelocityFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 4)));

    return res;
}

QString PacketParser::parse_REPORT_SOFTWARE_VERSION_INFO()
{
    QString s = "Получен пакет REPORT_SOFTWARE_VERSION_INFO (0x45):\n";
    QString info;
    info.append("---- Данные с уровня приложения:\n");
    info.append(QString("-- Major версия приложения: %0\n").arg((quint8)data[0]));
    info.append(QString("-- Minor версия приложения: %0\n").arg((quint8)data[1]));
    info.append(QString("-- Дата: %0/%1/%2\n").arg((quint8)data[3]).arg((quint8)data[2]).arg((quint8)data[4] + 1900));
    info.append("---- Данные с уровня ядра:\n");
    info.append(QString("-- Major версия ядра GPS: %0\n").arg((quint8)data[5]));
    info.append(QString("-- Minor версия ядра GPS: %0\n").arg((quint8)data[6]));
    info.append(QString("-- Дата: %0/%1/%2\n").arg((quint8)data[8]).arg((quint8)data[7]).arg((quint8)data[9] + 1900));
    s.append(info);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_REPORT_SOFTWARE_VERSION_INFO_label");
    qmlObject->setProperty("text", QVariant(info));

    return s;
}

QString PacketParser::parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL()
{
    QString message = "-- Уровни сигналов (пакет 0x47): \n";

    for (int i = 0; i < 12; i++) {
        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>(QString("template%0").arg(i + 1));
        if (!qmlObject) {
            qDebug() << "cannot find satellite object";
            return "";
        }
        qmlObject->setProperty("visible", "false");
    }

    // Нулевой байт уже разобранных данных - количество присланных пар "номер спутника - уровень сигнала".
    for (quint8 i = 0; i < (quint8)data[0]; i++) {
        float signalLevel = TypesConverter::bytesToSingle(data, i * 5 + 2);

        message.append(QString("Спутник %0: %1\n").arg((quint8)data[i * 5 + 1]).arg(signalLevel));

        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>(QString("template%0").arg(i + 1));
        qmlObject->setProperty("visible", "true");

        QObject *textObject = qmlObject->findChild<QObject *>("mainLabel");
        if (!textObject) {
            qDebug() << "no text object";
        }
        textObject->setProperty("text", QVariant(QString("Спутник %0: %1").arg((quint8)data[i * 5 + 1]).arg(signalLevel)));

        QObject *imageObject = qmlObject->findChild<QObject *>("sat_signal_image");
        if (!imageObject) {
            qDebug() << "no image object";
        }
        if (signalLevel > 50)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/excellent.png"));
        else if (signalLevel > 30)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/good.png"));
        else if (signalLevel > 15)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/fair.png"));
        else
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/weak.png"));
    }

    return message;
}

QString PacketParser::parse_REPORT_SINGLE_LLA_POS()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_LLA_POS (0x4A):\n");
    res.append(QString("- Позиция LLA: (%0 рад; %1 рад; %2 м)\n").arg(TypesConverter::bytesToSingle(data, 0))
               .arg(TypesConverter::bytesToSingle(data, 4)).arg(TypesConverter::bytesToSingle(data, 8)));
    res.append(QString("- Clock bias: %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));

    float latitude = TypesConverter::bytesToSingle(data, 0) * (180 / PI);
    float longitude = TypesConverter::bytesToSingle(data, 4) * (180 / PI);
    float altitude = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 широты").arg(latitude, 0, 'f').arg(latitude > 0 ? "северной" : "южной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 долготы").arg(longitude, 0, 'f').arg(longitude > 0 ? "восточной" : "западной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(altitude, 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(TypesConverter::bytesToSingle(data, 12), 0, 'f', 6)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 4)));
    return res;
}

QString PacketParser::parse_REPORT_REQUEST_IO_OPTIONS()
{
    QString s = "Получен пакет REPORT_REQUEST_IO_OPTIONS (0x55):\n";
    QString info = "-- Данные о позиционировании:\n";

    // Данные о позиционировании.
    info.append(QString("---- ECEF: %0\n").arg((data[0] & BIT0) ? "вкл" : "выкл"));
    info.append(QString("---- LLA: %0\n").arg((data[0] & BIT1) ? "вкл" : "выкл"));
    info.append(QString("---- HAE или MSL геоид: %0\n").arg(data[0] & BIT2 ? "MSL геоид" : "HAE"));
    info.append(QString("---- Точность: %0\n").arg(data[0] & BIT4 ? "double" : "single"));

    // Данные о скорости.
    info.append("-- Данные о скорости:\n");
    info.append(QString("---- ECEF: %0\n").arg((data[1] & BIT0) ? "вкл" : "выкл"));
    info.append(QString("---- ENU: %0\n").arg((data[1] & BIT1) ? "вкл" : "выкл"));

    // Данные о времени.
    info.append(QString("-- Тайминг: %0\n").arg((data[2] & BIT0) ? "UTC" : "GPS"));

    // Разное.
    info.append("-- Разное:\n");
    info.append(QString("---- Пакет 5А: %0\n").arg((data[3] & BIT0) ? "вкл" : "выкл"));
    info.append(QString("---- Единицы вывода: %0\n").arg((data[3] & BIT3) ? "dB-Hz" : "AMU"));
    s.append(info);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_REPORT_REQUEST_IO_OPTIONS_label");
    qmlObject->setProperty("text", QVariant(info));

    return s;
}

QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_ENU()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_VELOCITY_FIX_ENU (0x56):\n");
    res.append(QString("- Скорость (восток; север; по высоте): (%0 м/с; %1 м/с; %2 м/с)\n").arg(TypesConverter::bytesToSingle(data, 0))
               .arg(TypesConverter::bytesToSingle(data, 4)).arg(TypesConverter::bytesToSingle(data, 8)));
    res.append(QString("- Bias rate: %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
    res.append(QString("- Time of fix: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));

    float east_vel = TypesConverter::bytesToSingle(data, 0);
    float north_vel = TypesConverter::bytesToSingle(data, 4);
    float up_vel = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("eastVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("Скорость: %0 м/с на %1").arg(east_vel, 0, 'f', 3).arg(east_vel > 0 ? "восток" : "запад")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("northVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("          %0 м/с на %1").arg(north_vel, 0, 'f', 3).arg(north_vel > 0 ? "север" : "юг")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("upVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("          %0 м/с %1").arg(up_vel, 0, 'f', 3).arg(up_vel > 0 ? "вверх" : "вниз")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("enuVelocityBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м/с").arg(TypesConverter::bytesToSingle(data, 12), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("enuVelocityFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения: %0 сек").arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 4)));

    return res;
}

QString PacketParser::parse_REPORT_LAST_FIX_INFO()
{
    QString res;
    res.append("Получен пакет REPORT_LAST_FIX_INFO (0x57):\n");
    res.append(QString("- Источник информации: %0\n").arg((quint8)data[0] == (quint8)0 ? "старый фикс" : "новый фикс"));
    res.append(QString("- Режим фикса: "));
    switch ((quint8)data[1]) {
        case 0: res.append("нет предыдущего фикса"); break;
        case 1: res.append("только время, 1-SV"); break;
        case 2: res.append("2D clock hold"); break;
        case 3: res.append("2D"); break;
        case 4: res.append("3D"); break;
        case 5: res.append("переопределенный clock"); break;
        case 6: res.append("ссылка DGPS"); break;
        default: res.append("неизвестно");
    }
    res.append(QString("\n- Время последнего фикса, сек. GPS: %0\n").arg(TypesConverter::bytesToSingle(data, 2)));
    res.append(QString("- Неделя последнего фикса: %0\n").arg(TypesConverter::bytesToUInt16(data, 6)));

    return res;
}

QString PacketParser::parse_REPORT_GPS_SYSTEM_DATA()
{
    if ((int)data[0] == 3) {
        return QString("На спутнике %0 нет данных (пустой пакет REPORT_GPS_SYSTEM_DATA (0x58))").arg((quint8)data[2]);
    }

    QString s = "Получен пакет REPORT_GPS_SYSTEM_DATA (0x58):\n";
    switch ((quint8)data[1]) {

        case 2:     // Альманах
            s.append(QString("Данные альманаха для спутника %0:\n").arg((quint8)data[2]));
            s.append(QString("T_oa (raw): %0\n").arg((quint8)data[4]));
            s.append(QString("SV_HEALTH: %0\n").arg((quint8)data[5]));
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

        case (quint8)3:     // Здоровье
            s.append(QString("Данные о здоровье для спутников:\n"));
            s.append(QString("Номер недели для здоровья: %0\n").arg((quint8)data[4]));
            for (int i = 5; i <= 36; i++) {
                s.append(QString("Здоровье спутника %0: %1\n").arg(i - 4).arg((quint8)data[i]));
            }
            s.append(QString("t_oa для здоровья: %0\n").arg((quint8)data[37]));
            s.append(QString("Текущее t_oa: %0\n").arg((quint8)data[38]));
            s.append(QString("Текущий номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            break;

        case (quint8)4:     // Ионосфера
            s.append(QString("Данные об ионосфере для спутника %0:\n").arg((quint8)data[2]));
            s.append(QString("alpha0 = %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
            s.append(QString("alpha1 = %0\n").arg(TypesConverter::bytesToSingle(data, 16)));
            s.append(QString("alpha2 = %0\n").arg(TypesConverter::bytesToSingle(data, 20)));
            s.append(QString("alpha3 = %0\n").arg(TypesConverter::bytesToSingle(data, 24)));
            s.append(QString("beta0 = %0\n").arg(TypesConverter::bytesToSingle(data, 28)));
            s.append(QString("beta1 = %0\n").arg(TypesConverter::bytesToSingle(data, 32)));
            s.append(QString("beta2 = %0\n").arg(TypesConverter::bytesToSingle(data, 36)));
            s.append(QString("beta3 = %0\n").arg(TypesConverter::bytesToSingle(data, 40)));
            break;

        case (quint8)5:     // Информация UTC
            s.append(QString("Информация UTC для спутника %0:\n").arg((quint8)data[2]));
            s.append(QString("A0 = %0\n").arg(TypesConverter::bytesToDouble(data, 17)));
            s.append(QString("A1 = %0\n").arg(TypesConverter::bytesToSingle(data, 25)));
            s.append(QString("delta t_LS = %0\n").arg(TypesConverter::bytesToSInt16(data, 29)));
            s.append(QString("t_ot = %0\n").arg(TypesConverter::bytesToSingle(data, 31)));
            s.append(QString("WN_t = %0\n").arg(TypesConverter::bytesToUInt16(data, 35)));
            s.append(QString("WN_LSF = %0\n").arg(TypesConverter::bytesToUInt16(data, 37)));
            s.append(QString("DN = %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            s.append(QString("delta t_LSf = %0\n").arg(TypesConverter::bytesToSInt16(data, 41)));
            break;

        case (quint8)6:     // Информация ephemeris
            s.append(QString("Информация ephemeris для спутника %0:\n").arg((quint8)data[2]));
            s.append(QString("Номер спутника (SV PRN): %0\n").arg((quint8)data[4]));
            s.append(QString("t_ephem = %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
            s.append(QString("Номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 9)));
            s.append(QString("Точность спутника (raw): %0\n").arg((quint8)data[13]));
            s.append(QString("Здоровье спутника: %0\n").arg((quint8)data[14]));
            s.append(QString("Issue of data clock: %0\n").arg(TypesConverter::bytesToUInt16(data, 15)));
            s.append(QString("t_GD = %0\n").arg(TypesConverter::bytesToSingle(data, 17)));
            s.append(QString("t_oc = %0\n").arg(TypesConverter::bytesToSingle(data, 21)));
            s.append(QString("a_f2 = %0\n").arg(TypesConverter::bytesToSingle(data, 25)));
            s.append(QString("a_f1 = %0\n").arg(TypesConverter::bytesToSingle(data, 29)));
            s.append(QString("a_f0 = %0\n").arg(TypesConverter::bytesToSingle(data, 33)));
            s.append(QString("Точность спутника: %0\n").arg(TypesConverter::bytesToSingle(data, 37)));
            s.append(QString("Issue of data ephemeris: %0\n").arg((quint8)data[41]));
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
    QString s = "";
    if ((int)data[0] == 3) {
        // Пришла информация о включении спутников.
        s.append("Информация о включении спутников:\n");
        for (int i = 1; i <= 32; i++) {
            s.append(QString("- Спутник %0: %1\n").arg(i).arg((quint8)data[i] == (quint8)0 ? "включен" : "выключен"));
        }
    }
    else {
        // Пришла информация о принятии здоровь спутников во внимание.
        s.append("Информация о принятии здоровья спутников во внимание:\n");
        for (int i = 1; i <= 32; i++) {
            s.append(QString("- Спутник %0: %1\n").arg(i).arg((quint8)data[i] == (quint8)0 ? "принимается" : "игнорируется"));
        }
    }

    return s;
}

QString PacketParser::parse_REPORT_RAW_MEASUREMENT_DATA()
{
    QString s = "Получен пакет REPORT_RAW_MEASUREMENT_DATA (0x5A):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg((quint8)data[0]));
    s.append(QString("- Длина сэмпла, мс: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- Уровень сигнала: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- Кодовая фаза: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- Допплер: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));
    s.append(QString("- Время измерения, с: %0\n").arg(TypesConverter::bytesToDouble(data, 17)));

    return s;
}

QString PacketParser::parse_REPORT_SATELLITE_TRACKING_STATUS()
{
    QString s = "Получен пакет REPORT_SATELLITE_TRACKING_STATUS (0x5C):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg((quint8)data[0]));
    s.append(QString("- Номер канала: %0\n").arg(((quint8)data[1] & (BIT3 | BIT2 | BIT1 | BIT0)) + 1));
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
    // Здесь (BIT7 | BIT6 | BIT5) - маска старших трех битов,
    // (BIT3 | BIT2 | BIT1 | BIT0) - младших четырех битов.
    if ((((quint8)data[0] & (BIT7 | BIT6 | BIT5)) >> 5) == (quint8)1) {
        fix_dimension = "1D clock fix";
    }
    else if ((((quint8)data[0] & (BIT7 | BIT6 | BIT5)) >> 5) == (quint8)3) {
        fix_dimension = "2D fix";
    }
    else if ((((quint8)data[0] & (BIT7 | BIT6 | BIT5)) >> 5) == (quint8)4) {
        fix_dimension = "3D fix";
    }
    else if ((((quint8)data[0] & (BIT7 | BIT6 | BIT5)) >> 5) == (quint8)5) {
        fix_dimension = "OD clock fix";
    }
    s.append(QString("- Fix dimension: %0\n").arg(fix_dimension));

    s.append(QString("- Fix mode: %0\n").arg(((quint8)data[0] & BIT4) ? "manual" : "auto"));
    s.append(QString("- Количество спутников в fix: %0\n").arg((quint8)data[0] & (BIT3 | BIT2 | BIT1 | BIT0)));
    s.append(QString("- PDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- HDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- VDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- TDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));
    s.append(QString("Список всех включенных спутников (отрицательное значение означает, \
                    что спутник отвергнут для использования в алгоритме T-RAIM):\n"));
    for (int i = 17; i < 17 + ((quint8)data[0] & (BIT3 | BIT2 | BIT1 | BIT0)); i++) {
        s.append(QString("SV PRN: %0\n").arg((qint8)data[i]));
    }

    return s;
}

QString PacketParser::parse_RPTSUB_PRIMARY_TIMING_PACKET()
{
    QString s = "Получен пакет RPTSUB_PRIMARY_TIMING_PACKET (0x8F-AB):\n";
    s.append(QString("- Секунды недели GPS: %0\n").arg(TypesConverter::bytesToUInt32(data, 1)));
    s.append(QString("- Номер недели GPS: %0\n").arg(TypesConverter::bytesToUInt16(data, 5)));
    s.append(QString("- Сдвиг UTC, cек: %0\n").arg(TypesConverter::bytesToSInt16(data, 7)));
    s.append("- Флаги времени:\n");
    s.append(QString("-- Время GPS/UTC: %0\n").arg((quint8)data[9] & BIT0 ? "UTC" : "GPS"));
    s.append(QString("-- PPS GPS/UTC: %0\n").arg((quint8)data[9] & BIT1 ? "UTC" : "GPS"));
    s.append(QString("-- Время %0\n").arg((quint8)data[9] & BIT2 ? "не установлено" : "установлено"));
    s.append(QString("-- Информация UTC: %0\n").arg((quint8)data[9] & BIT3 ? "отсутствует" : "присутствует"));
    s.append(QString("-- Время от %0\n").arg((quint8)data[9] & BIT4 ? "пользователя" : "GPS"));
    s.append(QString("- Дата и время: %0/%1/%2 %3:%4:%5\n").arg((quint8)data[13]).arg((quint8)data[14]).arg(TypesConverter::bytesToUInt16(data, 15))
            .arg((quint8)data[12]).arg((quint8)data[11]).arg((quint8)data[10]));

    return s;
}

QString PacketParser::parse_RPTSUB_SUPPL_TIMING_PACKET()
{
    QString s = "Получен пакет RPTSUB_SUPPL_TIMING_PACKET (0x8F-AC):\n";

    s.append(QString("- Режим приемника: "));
    switch ((quint8)data[1]) {
        case 0: s.append("автоматический (2D/3D)"); break;
        case 1: s.append("один спутник (время)"); break;
        case 3: s.append("горизонтальный (2D)"); break;
        case 4: s.append("полная позиция (3D)"); break;
        case 7: s.append("переопределенный clock"); break;
        default: s.append("неизвестно (ошибка!)");
    }

    s.append(QString("\n- Режим дисциплинирования: "));
    switch ((quint8)data[2]) {
        case 0: s.append("обычный (закреплен за GPS)"); break;
        case 1: s.append("Включение"); break;
        case 2: s.append("автоудержание"); break;
        case 3: s.append("ручное удержание"); break;
        case 4: s.append("восстановление"); break;
        case 5: s.append("не используется"); break;
        case 6: s.append("дисциплинирование выключено"); break;
        default: s.append("неизвестно (ошибка!)");
    }

    s.append(QString("\n- Прогресс самоопроса, %: %0\n").arg((int)data[3] != 0 ? QString((quint8)data[3]) : QString("самоопрос в данный момент не проводится")));
    s.append(QString("- Длительность удержания (текущего или последнего, если удержание было отключено), сек: %0\n").arg(TypesConverter::bytesToUInt32(data, 4)));
    s.append("- Критические сигналы:\n");
    s.append(QString("-- DAC at rail: %0\n").arg((quint8)data[8] & BIT4 ? "ДА" : "нет"));
    s.append("- Не такие критические сигналы:\n");
    s.append(QString("-- DAC near rail: %0\n").arg((quint8)data[10] & BIT0 ? "ДА" : "нет"));
    s.append(QString("-- Малое напряжение на антенне (open): %0\n").arg((quint8)data[10] & BIT1 ? "ДА" : "нет"));
    s.append(QString("-- Слишком большое напряжение на антенне (short): %0\n").arg((quint8)data[10] & BIT2 ? "ДА" : "нет"));
    s.append(QString("-- Нет спутников для слежения: %0\n").arg((quint8)data[10] & BIT3 ? "ДА" : "нет"));
    s.append(QString("-- Нет дисциплинирования: %0\n").arg((quint8)data[10] & BIT4 ? "ДА" : "нет"));
    s.append(QString("-- Самоопрос в процессе: %0\n").arg((quint8)data[10] & BIT5 ? "ДА" : "нет"));
    s.append(QString("-- Нет сохраненной позиции: %0\n").arg((quint8)data[10] & BIT6 ? "ДА" : "нет"));
    s.append(QString("-- Leap second pending: %0\n").arg((quint8)data[10] & BIT7 ? "ДА" : "нет"));
    s.append(QString("-- В тестовом режиме: %0\n").arg((quint8)data[11] & BIT0 ? "ДА" : "нет"));
    s.append(QString("-- Позиция под вопросом: %0\n").arg((quint8)data[11] & BIT1 ? "ДА" : "нет"));
    s.append(QString("-- Неполный альманах: %0\n").arg((quint8)data[11] & BIT3 ? "ДА" : "нет"));
    s.append(QString("-- PPS не генерируется: %0\n").arg((quint8)data[11] & BIT4 ? "ДА" : "нет"));

    s.append(QString("- Статус декодирования GPS: "));
    switch ((quint8)data[12]) {
        case (quint8)0x00: s.append("делает фиксы"); break;
        case (quint8)0x01: s.append("не имеет времени GPS"); break;
        case (quint8)0x03: s.append("PDOP слишком высок"); break;
        case (quint8)0x08: s.append("нет юзабельных sat"); break;
        case (quint8)0x09: s.append("только один юзабельный sat"); break;
        case (quint8)0x0A: s.append("только два юзабельных sat"); break;
        case (quint8)0x0B: s.append("только три юзабельных sat"); break;
        case (quint8)0x0C: s.append("выбранный sat неюзабелен"); break;
        case (quint8)0x10: s.append("TRAIM отбросил фикс"); break;
        default: s.append("неизвестно (ошибка!)");
    }

    s.append(QString("\n- Дисциплинированная активность: "));
    switch ((quint8)data[13]) {
        case (quint8)0: s.append("лок фазы"); break;
        case (quint8)1: s.append("разогрев осциллятора"); break;
        case (quint8)2: s.append("лок частоты"); break;
        case (quint8)3: s.append("установка PPS"); break;
        case (quint8)4: s.append("инициализация повторяющего фильтра"); break;
        case (quint8)5: s.append("компенсация OCXO (удержание)"); break;
        case (quint8)6: s.append("неактивно"); break;
        case (quint8)7: s.append("не используется"); break;
        case (quint8)8: s.append("режим восстановления"); break;
        case (quint8)9: s.append("калибровка/управление напряжением"); break;
        default: s.append("неизвестно (ошибка!)");
    }

    s.append(QString("\n- Сдвиг PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));

    s.append(QString("- Сдвиг клока, ppb: %0\n").arg(TypesConverter::bytesToSingle(data, 20)));

    s.append(QString("- Значение DAC: %0\n").arg(TypesConverter::bytesToUInt32(data, 24)));

    s.append(QString("- Напряжение на DAC: %0V\n").arg(TypesConverter::bytesToSingle(data, 28)));

    s.append(QString("- Температура: %0 град. С\n").arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 2));

    s.append(QString("- Широта, рад: %0\n").arg(TypesConverter::bytesToDouble(data, 36), 0, 'f'));
    s.append(QString("- Долгота, рад: %0\n").arg(TypesConverter::bytesToDouble(data, 44), 0, 'f'));
    s.append(QString("- Высота, м: %0\n").arg(TypesConverter::bytesToDouble(data, 52), 0, 'f'));
    s.append(QString("- Ошибка квантизации PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 60), 0, 'f'));

    double latitude = TypesConverter::bytesToDouble(data, 36) * (180 / PI);
    double longitude = TypesConverter::bytesToDouble(data, 44) * (180 / PI);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("temperatureLabel");
    qmlObject->setProperty("text", QVariant(QString("Температура: %0 град. C").arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 широты").arg(latitude, 0, 'f').arg(latitude > 0 ? "северной" : "южной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 долготы").arg(longitude, 0, 'f').arg(longitude > 0 ? "восточной" : "западной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(TypesConverter::bytesToDouble(data, 52), 0, 'f', 4)));



/*
    QDataStream dataStream(data);
    //RPTSUB_SUPPL_TIMING_PACKET_report = new RPTSUB_SUPPL_TIMING_PACKET_reportStruct;
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream >> RPTSUB_SUPPL_TIMING_PACKET_report.reportSubcode
            >> RPTSUB_SUPPL_TIMING_PACKET_report.receiverMode >> RPTSUB_SUPPL_TIMING_PACKET_report.discipliningMode
            >> RPTSUB_SUPPL_TIMING_PACKET_report.selfSurveyProgress >> RPTSUB_SUPPL_TIMING_PACKET_report.holdoverDuration
            >> RPTSUB_SUPPL_TIMING_PACKET_report.criticalAlarmsBF >> RPTSUB_SUPPL_TIMING_PACKET_report.minorAlarmsBF
            >> RPTSUB_SUPPL_TIMING_PACKET_report.gpsDecodingStatus >> RPTSUB_SUPPL_TIMING_PACKET_report.discipliningActivity
            >> RPTSUB_SUPPL_TIMING_PACKET_report.spare1 >> RPTSUB_SUPPL_TIMING_PACKET_report.spare2
            >> RPTSUB_SUPPL_TIMING_PACKET_report.ppsOffset >> RPTSUB_SUPPL_TIMING_PACKET_report.clockOffset
            >> RPTSUB_SUPPL_TIMING_PACKET_report.dacValue >> RPTSUB_SUPPL_TIMING_PACKET_report.dacVoltage
            >> RPTSUB_SUPPL_TIMING_PACKET_report.temperature;
    dataStream.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dataStream >> RPTSUB_SUPPL_TIMING_PACKET_report.latitude
            >> RPTSUB_SUPPL_TIMING_PACKET_report.longitude >> RPTSUB_SUPPL_TIMING_PACKET_report.altitude;
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream >> RPTSUB_SUPPL_TIMING_PACKET_report.ppsQuantizationError;

*/
    return s;
}

/*void PacketParser::updateInterfaceValues()
{

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("temperatureLabel");
    qmlObject->setProperty("text", QVariant(QString("Температура: %0 град. C").arg(RPTSUB_SUPPL_TIMING_PACKET_report.temperature, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("Широта: %0").arg(RPTSUB_SUPPL_TIMING_PACKET_report.latitude * (180 / PI), 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("Долгота: %0").arg(RPTSUB_SUPPL_TIMING_PACKET_report.longitude * (180 / PI), 0, 'f')));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudeOutLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(RPTSUB_SUPPL_TIMING_PACKET_report.altitude, 0, 'f', 4)));

}
*/

QString PacketParser::parse_RPTSUB_PACKET_BROADCAST_MASK()
{
    QString s = "Получен пакет RPTSUB_PACKET_BROADCAST_MASK (0х8F-05):\n";
    s.append(QString("- Рассылка основных пакетов по таймингу: %0\n").arg(data[1] & BIT0 ? "ДА" : "нет"));
    s.append(QString("- Рассылка дополнительных пакетов по таймингу: %0\n").arg(data[1] & BIT2 ? "ДА" : "нет"));
    s.append(QString("- Рассылка других пакетов (позиция, скорость, сырые измерения, список спутников): %0\n").arg(data[1] & BIT6 ? "ДА" : "нет"));

    return s;
}