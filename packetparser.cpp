#include "packetparser.h"

// Конструктор, получает на вход пакет, который нужно будет разобрать (в виде байтов).
PacketParser::PacketParser(QByteArray data)
{
    this->data.clear();
    // Нулевой байт здесь - DLE, первый - код пакета.
    _reportCode = (quint8)data[1];
    // В массив data объекта попадет пакет без первого и последнего DLE и без кода пакета.
    for (int i = 2; i < data.length() - 1; i++) {
        this->data.append((quint8)data[i]);
    }
}

// Акцессор кода пакета.
quint8 PacketParser::reportCode()
{
    return _reportCode;
}

// Акцессор подкода пакета.
quint8 PacketParser::reportSubcode()
{
    return (quint8)data[0];
}

// Начинает процесс разбора, анализируя код и, если нужно, подкод пакета и выбирая
// подходящий метод разбора. Предназначен для запуска извне после создания пакета.
// Возвращает строку, которая будет отправлена в лог (в ней сообщение об успешном или
// неудачном разборе пакета и вся информация из пакета, которая не была передана в интерфейс
// самим методом разбора).
QString PacketParser::analyseAndParse()
{
    QString message = "";

    // Сначала подходящий метод ищется по коду пакета.
    switch (_reportCode) {
    case REPORT_UNPARSABLE:
        message.append(parse_REPORT_UNPARSABLE());
        break;
    case REPORT_FIRMWARE_INFO:
        // Иногда нужно проверить и подкод пакета тоже.
        switch (reportSubcode()) {
        case RPTSUB_FIRMWARE_VERSION:
            message.append(parse_RPTSUB_FIRMWARE_VERSION());
            break;
        case RPTSUB_HARDWARE_COMPONENT_INFO:
            message.append(parse_RPTSUB_HARDWARE_COMPONENT_INFO());
            break;
        default:
            message.append(QString("Подкод пакета REPORT_FIRMWARE_INFO (0х1С) %0 не распознан.")
                           .arg(reportSubcode(), 1, 16));
        }
        break;
    case REPORT_DOUBLE_XYZ_POS:
        message.append(parse_REPORT_DOUBLE_XYZ_POS());
        break;
    case REPORT_DOUBLE_LLA_POS:
        message.append(parse_REPORT_DOUBLE_LLA_POS());
        break;
    case REPORT_SINGLE_XYZ_POS:
        message.append(parse_REPORT_SINGLE_XYZ_POS());
        break;
    case REPORT_SINGLE_VELOCITY_FIX_XYZ:
        message.append(parse_REPORT_SINGLE_VELOCITY_FIX_XYZ());
        break;
    case REPORT_SOFTWARE_VERSION_INFO:
        message.append(parse_REPORT_SOFTWARE_VERSION_INFO());
        break;
    case REPORT_TRACKED_SATELLITES_SINGAL_LVL:
        message.append(parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL());
        break;
    case REPORT_SINGLE_LLA_POS:
        message.append(parse_REPORT_SINGLE_LLA_POS());
        break;
    case REPORT_REQUEST_IO_OPTIONS:
        message.append(parse_REPORT_REQUEST_IO_OPTIONS());
        break;
    case REPORT_SINGLE_VELOCITY_FIX_ENU:
        message.append(parse_REPORT_SINGLE_VELOCITY_FIX_ENU());
        break;
    case REPORT_LAST_FIX_INFO:
        message.append(parse_REPORT_LAST_FIX_INFO());
        break;
    case REPORT_GPS_SYSTEM_DATA:
        message.append(parse_REPORT_GPS_SYSTEM_DATA());
        break;
    case REPORT_STATUS_SATELLITE_HEALTH:
        message.append(parse_REPORT_STATUS_SATELLITE_HEALTH());
        break;
    case REPORT_RAW_MEASUREMENT_DATA:
        message.append(parse_REPORT_RAW_MEASUREMENT_DATA());
        break;
    case REPORT_SATELLITE_TRACKING_STATUS:
        message.append(parse_REPORT_SATELLITE_TRACKING_STATUS());
        break;
    case REPORT_SATELLITE_SELECTION_LIST:
        message.append(parse_REPORT_SATELLITE_SELECTION_LIST());
        break;
    case REPORT_SUPER:
        switch (reportSubcode()) {
        case RPTSUB_PRIMARY_TIMING_PACKET:
            message.append(parse_RPTSUB_PRIMARY_TIMING_PACKET());
            break;
        case RPTSUB_SUPPL_TIMING_PACKET:
            message.append(parse_RPTSUB_SUPPL_TIMING_PACKET());
            break;
        case RPTSUB_PACKET_BROADCAST_MASK:
            message.append(parse_RPTSUB_PACKET_BROADCAST_MASK());
            break;
        default:
            message.append(QString("Подкод пакета REPORT_SUPER (0х8F) %0 не распознан.")
                           .arg(reportSubcode(), 1, 16));
        }
        break;
    default:
        // Если код пакета никуда не подошел (всякое бывает)...
        message = QString("Неизвестный пакет 0x%0 ЛИБО проблемы с пониманием. Пакет отброшен")
                .arg(_reportCode, 1, 16);
    }

    return message;
}

// Разбирает пакет 0x13 и выводит его содержимое в лог.
// Этот пакет присылается GPS-модулем, если тот не понял, что отправили мы.
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

// Разбирает пакет получения информации о прошивке платы 0x1C с подкодом 0x81.
QString PacketParser::parse_RPTSUB_FIRMWARE_VERSION()
{
    QString res;
    QString info;

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x81)\n");
    info.append(QString("- Major версия прошивки: %0\n").arg((quint8)data[2]));
    info.append(QString("- Minor версия прошивки: %0\n").arg((quint8)data[3]));
    info.append(QString("- Номер сборки прошивки: %0\n").arg((quint8)data[4]));
    info.append(QString("- Дата сборки прошивки: %0/%1/%2\n")
                .arg((quint8)data[5]).arg((quint8)data[6]).arg(TypesConverter::bytesToUInt16(data, 7)));
    info.append(QString("- ID прошивки: "));
    for (quint8 i = 0; i < (quint8)data[9]; i++) {
        info.append((char)data[10 + i]);
    }

    // Вся информация будет передана в соответствующее поле в интерфейсе.
    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_RPTSUB_FIRMWARE_VERSION_label");
    qmlObject->setProperty("text", QVariant(info));

    return res;
}

// Разбирает пакет о железе платы 0x1C с подкодом 0x83.
QString PacketParser::parse_RPTSUB_HARDWARE_COMPONENT_INFO()
{
    QString res;
    QString info;

    res.append("Получен пакет RPTSUB_FIRMWARE_VERSION (0x1C / 0x83)\n");
    info.append(QString("- Серийный номер платы: %0\n").arg(TypesConverter::bytesToUInt32(data, 1)));
    info.append(QString("- Дата сборки платы: %0/%1/%2\n")
                .arg((quint8)data[5]).arg((quint8)data[6]).arg(TypesConverter::bytesToUInt16(data, 7)));
    info.append(QString("- Код платы, ассоциированный с ID: %0\n").arg(TypesConverter::bytesToUInt16(data, 10)));
    info.append(QString("- ID платы: "));
    for (quint8 i = 0; i < (quint8)data[12]; i++) {
        info.append((char)data[13 + i]);
    }

    // Вся информация из пакета отправляется в интерфейс.
    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_RPTSUB_HARDWARE_COMPONENT_INFO_label");
    qmlObject->setProperty("text", QVariant(info));

    return res;
}

// Разбирает пакет позиции в метрах двойной точности 0х83. Пакет приходит, как только новый фикс сделан
// и если соотв. настройки задают присылать его.
QString PacketParser::parse_REPORT_DOUBLE_XYZ_POS()
{
    QString res;
    res.append("Получен пакет REPORT_DOUBLE_XYZ_POS (0x83)\n");

    double X = TypesConverter::bytesToDouble(data, 0);
    double Y = TypesConverter::bytesToDouble(data, 8);
    double Z = TypesConverter::bytesToDouble(data, 16);

    // В интерфейсе есть текстовые поля под данные из этого пакета.
    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Х: %0 м").arg(X, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("yPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Y: %0 м").arg(Y, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("zPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Z: %0 м").arg(Z, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(fabs(TypesConverter::bytesToSingle(data, 24)), 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 0)));

    return res;
}

// Разбирает пакет позиции в радианах (широта, долгота, высота) двойной точности 0х84.
//Пакет приходит, как только новый фикс сделан
// и если соотв. настройки задают присылать его.
QString PacketParser::parse_REPORT_DOUBLE_LLA_POS()
{
    QString res;
    res.append("Получен пакет REPORT_DOUBLE_LLA_POS (0x84)\n");

    // В интерфейсе есть текстовые поля под данные из этого пакета.
    double latitude = TypesConverter::bytesToDouble(data, 0) * (180 / PI);
    double longitude = TypesConverter::bytesToDouble(data, 8) * (180 / PI);
    double altitude = TypesConverter::bytesToDouble(data, 16);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 широты")
                                            .arg(fabs(latitude), 0, 'f').arg(latitude > 0 ? "северной" : "южной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 долготы")
                                            .arg(fabs(longitude), 0, 'f').arg(longitude > 0 ? "восточной" : "западной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(altitude, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м")
                                            .arg(fabs(TypesConverter::bytesToSingle(data, 24)), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 0)));

    return res;
}

// Разбирает пакет позиции в метрах одинарной точности 0х42. Пакет приходит, как только новый фикс сделан
// и если соотв. настройки задают присылать его.
QString PacketParser::parse_REPORT_SINGLE_XYZ_POS()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_XYZ_POS (0x42)\n");

    float X = TypesConverter::bytesToSingle(data, 0);
    float Y = TypesConverter::bytesToSingle(data, 4);
    float Z = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Х: %0 м").arg(X, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("yPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Y: %0 м").arg(Y, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("zPositionLabel");
    qmlObject->setProperty("text", QVariant(QString("Позиция по Z: %0 м").arg(Z, 0, 'f', 2)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(fabs(TypesConverter::bytesToSingle(data, 8)), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 12), 0, 'f', 0)));

    return res;
}

// Разбирает пакет скорости в метрах в секунду одинарной точности 0х43. Пакет приходит, как только новый фикс сделан
// и если соотв. настройки задают присылать его.
QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_XYZ()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_VELOCITY_FIX_XYZ (0x43)\n");

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
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м/с").arg(fabs(TypesConverter::bytesToSingle(data, 12)), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("xyzVelocityFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 0)));

    return res;
}

// Пакет 0х45: версии приложения и ядра.
QString PacketParser::parse_REPORT_SOFTWARE_VERSION_INFO()
{
    QString s = "Получен пакет REPORT_SOFTWARE_VERSION_INFO (0x45)\n";
    QString info;
    info.append("---- Данные с уровня приложения:\n");
    info.append(QString("-- Major версия приложения: %0\n").arg((quint8)data[0]));
    info.append(QString("-- Minor версия приложения: %0\n").arg((quint8)data[1]));
    info.append(QString("-- Дата: %0/%1/%2\n").arg((quint8)data[3]).arg((quint8)data[2]).arg((quint8)data[4] + 1900));
    info.append("---- Данные с уровня ядра:\n");
    info.append(QString("-- Major версия ядра GPS: %0\n").arg((quint8)data[5]));
    info.append(QString("-- Minor версия ядра GPS: %0\n").arg((quint8)data[6]));
    info.append(QString("-- Дата: %0/%1/%2\n").arg((quint8)data[8]).arg((quint8)data[7]).arg((quint8)data[9] + 1900));

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_REPORT_SOFTWARE_VERSION_INFO_label");
    qmlObject->setProperty("text", QVariant(info));

    return s;
}

// В этом пакете 0х47 - уровни сигналов спутников. Приходит каждую секунду (если соотв. галочка стоит).
QString PacketParser::parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL()
{
    QString message = "Получен пакет REPORT_TRACKED_SATELLITES_SINGAL_LVL (0x47)\n";

    // Максимальное количество пар "спутник - сигнал" в пакете, согласно документации, равно 12.
    // В интерфейсе заранее выделено 12 полей-объектов для отображения. Сначала скроем их все:
    for (int i = 0; i < 12; i++) {
        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>(QString("template%0").arg(i + 1));
        qmlObject->setProperty("visible", "false");
    }

    // Нулевой байт уже разобранных данных - количество присланных пар "номер спутника - уровень сигнала".
    for (quint8 i = 0; i < (quint8)data[0]; i++) {
        float signalLevel = TypesConverter::bytesToSingle(data, i * 5 + 2);
        // Для спутника, у которого есть данные о сигнале, покажем заготовку в интерфейсе.
        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>(QString("template%0").arg(i + 1));
        qmlObject->setProperty("visible", "true");

        // Обновление информации об уровне сигнала в заготовке.
        QObject *qmlIOOptionsObject = QMLDataHelper::mainWindow->findChild<QObject *>("_REPORT_REQUEST_IO_OPTIONS_label");
        bool is_db_HZ = qmlIOOptionsObject->property("text").toString().contains("dB-Hz");
        QObject *textObject = qmlObject->findChild<QObject *>("mainLabel");
        textObject->setProperty("text", QVariant(QString("Спутник %0: %1 %2").arg((quint8)data[i * 5 + 1]).arg(signalLevel)
                                .arg(is_db_HZ ? "dBc" : "а.е.м.")));

        // Задание изображения соответственно уровню сигнала.
        float AMUfactor = (is_db_HZ ? 1 : 0.25);
        QObject *imageObject = qmlObject->findChild<QObject *>("sat_signal_image");
        if (signalLevel > 40 * AMUfactor)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/excellent.png"));
        else if (signalLevel > 30 * AMUfactor)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/good.png"));
        else if (signalLevel > 15 * AMUfactor)
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/fair.png"));
        else
            imageObject->setProperty("source", QVariant("qrc:/images/satellites_levels/weak.png"));
    }

    return message;
}

// Одинарная позиция широта-долгота-высота 0х4А. Приходит, как только новый фикс сделан
// (и если включен прием этих пакетов и соотв. точность).
QString PacketParser::parse_REPORT_SINGLE_LLA_POS()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_LLA_POS (0x4A)\n");

    float latitude = TypesConverter::bytesToSingle(data, 0) * (180 / PI);
    float longitude = TypesConverter::bytesToSingle(data, 4) * (180 / PI);
    float altitude = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("latitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 широты")
                                            .arg(fabs(latitude), 0, 'f').arg(latitude > 0 ? "северной" : "южной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("longitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("%0 градусов %1 долготы")
                                            .arg(fabs(longitude), 0, 'f').arg(longitude > 0 ? "восточной" : "западной")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("altitudePosLabel");
    qmlObject->setProperty("text", QVariant(QString("Высота над уровнем моря: %0 м").arg(altitude, 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м").arg(fabs(TypesConverter::bytesToSingle(data, 12)), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("llaPositionFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 0)));
    return res;
}

// Приходит после отправки или запроса настроек ввода-вывода GPS. Пакет 0х55.
QString PacketParser::parse_REPORT_REQUEST_IO_OPTIONS()
{
    QString s = "Получен пакет REPORT_REQUEST_IO_OPTIONS (0x55)\n";
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

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("_REPORT_REQUEST_IO_OPTIONS_label");
    qmlObject->setProperty("text", QVariant(info));

    return s;
}

// Пакет 0х56 одинарной скорости в м/с. Пакет приходит, как только новый фикс сделан
// и если соотв. настройки задают присылать его.
QString PacketParser::parse_REPORT_SINGLE_VELOCITY_FIX_ENU()
{
    QString res;
    res.append("Получен пакет REPORT_SINGLE_VELOCITY_FIX_ENU (0x56)\n");
    float east_vel = TypesConverter::bytesToSingle(data, 0);
    float north_vel = TypesConverter::bytesToSingle(data, 4);
    float up_vel = TypesConverter::bytesToSingle(data, 8);

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("eastVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("Скорость: %0 м/с на %1")
                                            .arg(fabs(east_vel), 0, 'f', 3).arg(east_vel > 0 ? "восток" : "запад")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("northVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("                 %0 м/с на %1")
                                            .arg(fabs(north_vel), 0, 'f', 3).arg(north_vel > 0 ? "север" : "юг")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("upVelocityLabel");
    qmlObject->setProperty("text", QVariant(QString("                 %0 м/с %1").arg(fabs(up_vel), 0, 'f', 3).arg(up_vel > 0 ? "вверх" : "вниз")));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("enuVelocityBiasLabel");
    qmlObject->setProperty("text", QVariant(QString("Погрешность: %0 м/с").arg(fabs(TypesConverter::bytesToSingle(data, 12)), 0, 'f', 4)));
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("enuVelocityFixLabel");
    qmlObject->setProperty("text", QVariant(QString("Отметка времени измерения:\n%0 сек")
                                            .arg(TypesConverter::bytesToSingle(data, 16), 0, 'f', 0)));

    return res;
}

// Не отображается в интерфейсе. Пакет 0х57 со служебной информацией о последнем фиксе позиции/скорости.
// Приходит вместе с запросом позиции и скорости (и прочей информации, которую оператор указал в настройках присылать).
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

// Не отображается в интерфейсе по причине гигантских размеров, полностью высыпается в лог.
// Пакет 0х58, может содержать либо альманах выбранного спутника, либо данные о работоспособности (состоянии, здоровье)
// всех спутников, либо данные об ионосфере, либо данные о UTC, либо данные эфемериды выбранного спутника.
// Много математических параметров, я совершенно не представляю, для чего они могут пригодиться, но специалистам
// по геолокации виднее.
QString PacketParser::parse_REPORT_GPS_SYSTEM_DATA()
{
    if ((int)data[0] == 3) {
        return QString("На спутнике %0 нет данных (пустой пакет REPORT_GPS_SYSTEM_DATA (0x58))").arg((quint8)data[2]);
    }

    QString s = "Получен пакет REPORT_GPS_SYSTEM_DATA (0x58):\n";
    switch ((quint8)data[1]) {

        case 2:     // Альманах спутника.
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

        case (quint8)3:     // Работоспособность спутников.
            s.append(QString("Данные о работоспособности спутников:\n"));
            s.append(QString("Номер недели данных о работоспособности: %0\n").arg((quint8)data[4]));
            for (int i = 5; i <= 36; i++) {
                s.append(QString("Работоспособность спутника %0: %1\n").arg(i - 4).arg((quint8)data[i]));
            }
            s.append(QString("t_oa работоспособности: %0\n").arg((quint8)data[37]));
            s.append(QString("Текущее t_oa: %0\n").arg((quint8)data[38]));
            s.append(QString("Текущий номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            break;

        case (quint8)4:     // Ионосфера.
            s.append(QString("Данные об ионосфере:\n"));
            s.append(QString("alpha0 = %0\n").arg(TypesConverter::bytesToSingle(data, 12)));
            s.append(QString("alpha1 = %0\n").arg(TypesConverter::bytesToSingle(data, 16)));
            s.append(QString("alpha2 = %0\n").arg(TypesConverter::bytesToSingle(data, 20)));
            s.append(QString("alpha3 = %0\n").arg(TypesConverter::bytesToSingle(data, 24)));
            s.append(QString("beta0 = %0\n").arg(TypesConverter::bytesToSingle(data, 28)));
            s.append(QString("beta1 = %0\n").arg(TypesConverter::bytesToSingle(data, 32)));
            s.append(QString("beta2 = %0\n").arg(TypesConverter::bytesToSingle(data, 36)));
            s.append(QString("beta3 = %0\n").arg(TypesConverter::bytesToSingle(data, 40)));
            break;

        case (quint8)5:     // Информация UTC.
            s.append(QString("Информация UTC:\n"));
            s.append(QString("A0 = %0\n").arg(TypesConverter::bytesToDouble(data, 17)));
            s.append(QString("A1 = %0\n").arg(TypesConverter::bytesToSingle(data, 25)));
            s.append(QString("delta t_LS = %0\n").arg(TypesConverter::bytesToSInt16(data, 29)));
            s.append(QString("t_ot = %0\n").arg(TypesConverter::bytesToSingle(data, 31)));
            s.append(QString("WN_t = %0\n").arg(TypesConverter::bytesToUInt16(data, 35)));
            s.append(QString("WN_LSF = %0\n").arg(TypesConverter::bytesToUInt16(data, 37)));
            s.append(QString("DN = %0\n").arg(TypesConverter::bytesToUInt16(data, 39)));
            s.append(QString("delta t_LSf = %0\n").arg(TypesConverter::bytesToSInt16(data, 41)));
            break;

        case (quint8)6:     // Информация эфемериды спутника.
            s.append(QString("Информация эфемериды спутника %0:\n").arg((quint8)data[2]));
            s.append(QString("Номер спутника (SV PRN): %0\n").arg((quint8)data[4]));
            s.append(QString("t_ephem = %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
            s.append(QString("Номер недели: %0\n").arg(TypesConverter::bytesToUInt16(data, 9)));
            s.append(QString("Точность спутника (raw): %0\n").arg((quint8)data[13]));
            s.append(QString("Работоспособность спутника: %0\n").arg((quint8)data[14]));
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

// Пакет 0х59, приходит с одним из видов информации в ответ на пакет 0х39 (тот пакет может иметь
// шесть разных подкодов, на два из них GPS-модуль ответит пакетом ниже). В пакете либо информация
// о включении спутников в расчеты GPS-модуля (по умолчанию включены все, но можно сделать так, чтобы
// GPS-модуль не воспринимал некоторые спутники) либо о слежении за работоспособностью спутников
// (можно отключить слежение, чтобы работать и со спутниками, которые, по их собственному мнению,
// не очень правильно работают, но это опасно).
// Вся информация в итоге выведется в интерфейс.
QString PacketParser::parse_REPORT_STATUS_SATELLITE_HEALTH()
{
    QString s = "";
    QString info = "";
    if ((int)data[0] == 3) {
        // Пришла информация о включении спутников.
        s.append("Информация о включении спутников REPORT_STATUS_SATELLITE_HEALTH\n");
        for (int i = 1; i <= 32; i++) {
            info.append(QString("№%0 - %1  \t\t").arg(i).arg((quint8)data[i] == (quint8)0 ? "вкл." : "выкл."));
            if (i % 4 == 0)
                info.append("\n");
        } 
        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("enabledSVsLabel");
        qmlObject->setProperty("text", QVariant(info));

    }
    else {
        // Пришла информация о принятии работоспособности спутников во внимание.
        s.append("Информация о принятии работоспособности спутников во внимание REPORT_STATUS_SATELLITE_HEALTH\n");
        for (int i = 1; i <= 32; i++) {
            info.append(QString("№%0 - %1\t").arg(i).arg((quint8)data[i] == (quint8)0 ? "учитыв." : "игнор."));
            if (i % 4 == 0)
                info.append("\n");
        }
        QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("healthEnabledSVsLabel");
        qmlObject->setProperty("text", QVariant(info));
    }

    return s;
}

// Не отображается в интерфейсе. Какие-то совсем уж служебные параметры низкого уровня об измерениях спутника
// (или GPS-модуля). Пакет 0х5А, тоже присылается в момент фикса, если стоит соотв. галочка в настройках I/O.
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

// Не отображается в интерфейсе. Пакет 0х5С о статусе трекинга спутника. Если на вкладке с кнопкой, вызывающей
// приход этого пакета, выбран номер спутника "0", то информация придет о всех спутниках. Что содержит пакет, можно посмотреть ниже.
QString PacketParser::parse_REPORT_SATELLITE_TRACKING_STATUS()
{
    QString s = "Получен пакет REPORT_SATELLITE_TRACKING_STATUS (0x5C):\n";
    s.append(QString("- Номер спутника (SV PRN): %0\n").arg((quint8)data[0]));
    int channelNo = 1;
    if ((quint8)data[1] & BIT3) channelNo += 1;
    if ((quint8)data[1] & BIT2) channelNo += 2;
    if ((quint8)data[1] & BIT1) channelNo += 4;
    if ((quint8)data[1] & BIT0) channelNo += 8;
    s.append(QString("- Номер канала: %0\n").arg(channelNo));
    s.append(QString("- Acquisition flag: %0\n")
             .arg((int)data[2] == 0 ? "never acquired" : ((int)data[2] == 1 ? "acquired" : "re-opened search")));
    s.append(QString("- Флаг эфемериды: %0\n").arg((int)data[3] == 0 ? "флаг не установлен" : "хорошая эфемерида"));
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
        bad_data_flag = "Испорченная эфемерида";
    }
    s.append(QString("Флаг испорченных данных: %0\n").arg(bad_data_flag));

    s.append(QString("- Флаг сбора данных: %0\n").arg((int)data[23] == 0 ? "флаг не установлен" : "сбор в процессе"));

    return s;
}

// Из этого пакета о списке трекинга спутников 0х6D часть попадает в интерфейс - количество спутников в фиксе
// Часть высылается в лог. Присылается каждую секунду, если галочка стоит.
QString PacketParser::parse_REPORT_SATELLITE_SELECTION_LIST()
{
    QString s = "Получен пакет REPORT_SATELLITE_SELECTION_LIST (0x6D):\n";
    QString fix_dimension = "";
    // Здесь (BIT7 | BIT6 | BIT5) - маска старших трех битов,
    // (BIT3 | BIT2 | BIT1 | BIT0) - младших четырех битов.
    // Биты расположены так, что нельзя просто образовать маску логическим сложением и получить правильное число
    // из этих битов. Приходится разворачивать вручную...
    int fix_dimension_value = 0;
    if ((quint8)data[0] & BIT7)
        fix_dimension_value += 1;
    if ((quint8)data[0] & BIT6)
        fix_dimension_value += 2;
    if ((quint8)data[0] & BIT5)
        fix_dimension_value += 4;
    if (fix_dimension_value == 1) {
        fix_dimension = "1D clock fix";
    }
    else if (fix_dimension_value == 3) {
        fix_dimension = "2D fix";
    }
    else if (fix_dimension_value == 4) {
        fix_dimension = "3D fix";
    }
    else if (fix_dimension_value == 5) {
        fix_dimension = "OD clock fix";
    }
    s.append(QString("- Fix dimension: %0\n").arg(fix_dimension));

    s.append(QString("- Fix mode: %0\n").arg(((quint8)data[0] & BIT4) ? "manual" : "auto"));

    int svsNumber = 0;
    if ((quint8)data[0] & BIT3)
        svsNumber += 1;
    if ((quint8)data[0] & BIT2)
        svsNumber += 2;
    if ((quint8)data[0] & BIT1)
        svsNumber += 4;
    if ((quint8)data[0] & BIT0)
        svsNumber += 8;
    s.append(QString("- Количество спутников в fix: %0\n").arg(svsNumber));
    s.append(QString("- PDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 1)));
    s.append(QString("- HDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 5)));
    s.append(QString("- VDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 9)));
    s.append(QString("- TDOP: %0\n").arg(TypesConverter::bytesToSingle(data, 13)));

    QString fixedSvs = QString("%0 спутников в фиксе: ").arg(svsNumber);
    for (int i = 17; i < 17 + svsNumber; i++) {
            fixedSvs.append(QString("%0").arg((qint8)data[i]));
            if (i != 17 + svsNumber - 1)
                fixedSvs.append(", ");
    }

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("fixedSVsLabel");
    qmlObject->setProperty("text", QVariant(fixedSvs));

    return s;
}

// Основной суперпакет по таймингу 0х8F 0хАВ. Присылается по умолчанию каждую секунду. Из него в интерфейс пишется время.
// Всё остальное отправляется в лог. Как обычно, содержимое расписано ниже.
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

    QString timeString = "";
    // Хак для корректировки часового пояса.
    quint8 hour = (quint8)data[12] + 3;
    quint8 date = (quint8)data[13];
    if (hour >= 24) {
        hour -= 24;
        date += 1;
    }
    timeString.append(QString("- Дата и время: %0/%1/%2 %3:%4:%5")
            .arg(date, 2, 10, QChar('0')).arg((quint8)data[14], 2, 10, QChar('0')).arg(TypesConverter::bytesToUInt16(data, 15))
            .arg(hour, 2, 10, QChar('0')).arg((quint8)data[11], 2, 10, QChar('0')).arg((quint8)data[10], 2, 10, QChar('0')));

    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("timeLabel");
    qmlObject->setProperty("text", QVariant(QString("%1 %0").arg((quint8)data[9] & BIT0 ? "UTC" : "GPS").arg(timeString)));

    return s;
}

// Дополнительный пакет по таймингу 0х8F 0xAC. Присылается по умолчанию каждую секунду. Обновляет в интерфейсе
// данные температуры, вольтажа, а также критические и не очень критические сигналы (и еще немного информации о GPS-модуле).
// Всё остальное сыплется в лог.
// Этот пакет и пакет выше можно прислать и принудительно, есть спец. вкладка в интерфейсе.
QString PacketParser::parse_RPTSUB_SUPPL_TIMING_PACKET()
{
    QString s = "Получен пакет RPTSUB_SUPPL_TIMING_PACKET (0x8F-AC):\n";
    QString info = "";
    info.append(QString("- Режим приемника: "));
    switch ((quint8)data[1]) {
        case 0: info.append("автоматический (2D/3D)"); break;
        case 1: info.append("один спутник (время)"); break;
        case 3: info.append("горизонтальный (2D)"); break;
        case 4: info.append("полная позиция (3D)"); break;
        case 7: info.append("переопределенный clock"); break;
        default: info.append("неизвестно (ошибка!)");
    }

    info.append(QString("\n- Режим дисциплинирования: "));
    switch ((quint8)data[2]) {
        case 0: info.append("обычный (закреплен за GPS)"); break;
        case 1: info.append("Включение"); break;
        case 2: info.append("автоудержание"); break;
        case 3: info.append("ручное удержание"); break;
        case 4: info.append("восстановление"); break;
        case 5: info.append("не используется"); break;
        case 6: info.append("дисциплинирование выключено"); break;
        default: info.append("неизвестно (ошибка!)");
    }

    s.append(QString("\n- Прогресс самоопроса, %: %0\n").arg((quint8)data[3]));
    s.append(QString("- Длительность удержания (текущего или последнего, если удержание было отключено), сек: %0\n")
             .arg(TypesConverter::bytesToUInt32(data, 4)));
    info.append("\n- Критические сигналы:\n");
    info.append(QString("-- DAC at rail: %0\n").arg((quint8)data[8] & BIT4 ? "ДА" : "нет"));
    info.append("- Не такие критические сигналы:\n");
    info.append(QString("-- DAC near rail: %0\n").arg((quint8)data[10] & BIT0 ? "ДА" : "нет"));
    info.append(QString("-- Малое напряжение на антенне (open): %0\n").arg((quint8)data[10] & BIT1 ? "ДА" : "нет"));
    info.append(QString("-- Слишком большое напряжение на антенне (short): %0\n").arg((quint8)data[10] & BIT2 ? "ДА" : "нет"));
    info.append(QString("-- Нет спутников для слежения: %0\n").arg((quint8)data[10] & BIT3 ? "ДА" : "нет"));
    info.append(QString("-- Нет дисциплинирования: %0\n").arg((quint8)data[10] & BIT4 ? "ДА" : "нет"));
    info.append(QString("-- Самоопрос в процессе: %0\n").arg((quint8)data[10] & BIT5 ? "ДА" : "нет"));
    info.append(QString("-- Нет сохраненной позиции: %0\n").arg((quint8)data[10] & BIT6 ? "ДА" : "нет"));
    info.append(QString("-- Leap second pending: %0\n").arg((quint8)data[10] & BIT7 ? "ДА" : "нет"));
    info.append(QString("-- В тестовом режиме: %0\n").arg((quint8)data[11] & BIT0 ? "ДА" : "нет"));
    info.append(QString("-- Позиция под вопросом: %0\n").arg((quint8)data[11] & BIT1 ? "ДА" : "нет"));
    info.append(QString("-- Неполный альманах: %0\n").arg((quint8)data[11] & BIT3 ? "ДА" : "нет"));
    info.append(QString("-- PPS не генерируется: %0\n").arg((quint8)data[11] & BIT4 ? "ДА" : "нет"));

    info.append(QString("- Статус декодирования GPS: "));
    switch ((quint8)data[12]) {
        case (quint8)0x00: info.append("делает фиксы"); break;
        case (quint8)0x01: info.append("не имеет времени GPS"); break;
        case (quint8)0x03: info.append("PDOP слишком высок"); break;
        case (quint8)0x08: info.append("нет юзабельных sat"); break;
        case (quint8)0x09: info.append("только один юзабельный sat"); break;
        case (quint8)0x0A: info.append("только два юзабельных sat"); break;
        case (quint8)0x0B: info.append("только три юзабельных sat"); break;
        case (quint8)0x0C: info.append("выбранный sat неюзабелен"); break;
        case (quint8)0x10: info.append("TRAIM отбросил фикс"); break;
        default: info.append("неизвестно (ошибка!)");
    }

    info.append(QString("\n- Дисциплинированная активность: "));
    switch ((quint8)data[13]) {
        case (quint8)0: info.append("лок фазы"); break;
        case (quint8)1: info.append("разогрев осциллятора"); break;
        case (quint8)2: info.append("лок частоты"); break;
        case (quint8)3: info.append("установка PPS"); break;
        case (quint8)4: info.append("инициализация повторяющего фильтра"); break;
        case (quint8)5: info.append("компенсация OCXO (удержание)"); break;
        case (quint8)6: info.append("неактивно"); break;
        case (quint8)7: info.append("не используется"); break;
        case (quint8)8: info.append("режим восстановления"); break;
        case (quint8)9: info.append("калибровка/управление напряжением"); break;
        default: info.append("неизвестно (ошибка!)");
    }
    QObject *qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("supplInfoLabel");
    qmlObject->setProperty("text", QVariant(info));

    s.append(QString("\n- Сдвиг PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 16)));

    s.append(QString("- Сдвиг клока, ppb: %0\n").arg(TypesConverter::bytesToSingle(data, 20)));

    s.append(QString("- Значение DAC: %0\n").arg(TypesConverter::bytesToUInt32(data, 24)));

    QString DACV = "";
    DACV.append(QString("- Напряжение на DAC: %0V\n").arg(TypesConverter::bytesToSingle(data, 28), 0, 'f', 4));
    s.append(DACV);
    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("dacVoltageLabel");
    qmlObject->setProperty("text", QVariant(DACV));

    s.append(QString("- Температура: %0 град. С\n").arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 2));

    s.append(QString("- Широта из памяти, рад: %0\n").arg(TypesConverter::bytesToDouble(data, 36), 0, 'f'));
    s.append(QString("- Долгота из памяти, рад: %0\n").arg(TypesConverter::bytesToDouble(data, 44), 0, 'f'));
    s.append(QString("- Высота из памяти, м: %0\n").arg(TypesConverter::bytesToDouble(data, 52), 0, 'f'));
    s.append(QString("- Ошибка квантизации PPS, нс: %0\n").arg(TypesConverter::bytesToSingle(data, 60), 0, 'f'));

    qmlObject = QMLDataHelper::mainWindow->findChild<QObject *>("temperatureLabel");
    qmlObject->setProperty("text", QVariant(QString("- Температура: %0 град. C")
                                            .arg(TypesConverter::bytesToSingle(data, 32), 0, 'f', 2)));
    return s;
}

// Не отображается в интерфейсе (GPS-модуль тупо присылает то, что отправляет пользователь,
// даже если отправленная маска не работает).
// Пакет-ответ на задание маски авторассылки пакетов 0x8F 0xAB, 0x8F 0xAC, 0x47
// и еще некоторые (в документации есть хорошая таблица на этот счет).
QString PacketParser::parse_RPTSUB_PACKET_BROADCAST_MASK()
{
    QString s = "Получен пакет RPTSUB_PACKET_BROADCAST_MASK (0х8F-05):\n";
    s.append(QString("- Рассылка основных пакетов по таймингу: %0\n").arg(data[1] & BIT0 ? "ДА" : "нет"));
    s.append(QString("- Рассылка дополнительных пакетов по таймингу: %0\n").arg(data[1] & BIT2 ? "ДА" : "нет"));
    s.append(QString("- Рассылка других пакетов (позиция, скорость, сырые измерения, список спутников): %0\n")
             .arg(data[1] & BIT6 ? "ДА" : "нет"));

    return s;
}