#include "cmdbuilder.h"

// Метод собирает все опции пакета установки опций ввода-вывода с соответствующей вкладки интерфейса.
// Все эти опции перечислены в документации, см. пакет 0х35.
void CommandBuilder::build_COMMAND_SET_IO_OPTIONS(QByteArray *cmd)
{
    // Опции считываются из интерфейса с помощью обращения по объектному имени элементов QML.
    bool ecefChecked = QMLDataHelper::getBoolFromQML("eCEFcheck", "checked");
    bool llaChecked = QMLDataHelper::getBoolFromQML("lLAcheck", "checked");
    bool precision = QMLDataHelper::getBoolFromQML("doublePrecRB", "checked");
    bool gpsTime = QMLDataHelper::getBoolFromQML("gpsTimeRB", "checked");
    bool mslChecked = QMLDataHelper::getBoolFromQML("mslRB", "checked");
    bool ecefVelChecked = QMLDataHelper::getBoolFromQML("ecef_vel_check", "checked");
    bool enuVelChecked = QMLDataHelper::getBoolFromQML("enu_vel_check", "checked");
    bool raw_data_report_checked = QMLDataHelper::getBoolFromQML("raw_data_report_check", "checked");
    bool dbhz_out_checked = QMLDataHelper::getBoolFromQML("dbhz_out_RB", "checked");

    // Принятые из интерфейса опции нужно собрать в битовые поля согласно, опять же, документации.
    QByteArrayHelper::appendAndStuff(cmd, (quint8)(
               (ecefChecked ? BIT0 : 0) |
               (llaChecked ? BIT1 : 0) |
               (mslChecked ? BIT2 : 0) |
               (precision ? BIT4 : 0)
              ));
    QByteArrayHelper::appendAndStuff(cmd, (quint8)(
               (ecefVelChecked ? BIT0 : 0) |
               (enuVelChecked ? BIT1 : 0)
              ));
    QByteArrayHelper::appendAndStuff(cmd, (quint8)(gpsTime ? 0 : BIT0));
    QByteArrayHelper::appendAndStuff(cmd, (quint8)(
               (raw_data_report_checked ? BIT0 : 0) |
               (dbhz_out_checked ? BIT3 : 0)
              ));

    qDebug() << *cmd;
}

// Этот метод и метод ниже помещают в пакет инициализации начальной позиции
// значения начальной позиции, напечатанные пользователем в соответствующей вкладке
// интерфейса.
void CommandBuilder::build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *cmd)
{
    double initX = QMLDataHelper::getDoubleFromQML("init_x_text", "text");
    double initY = QMLDataHelper::getDoubleFromQML("init_y_text", "text");
    double initZ = QMLDataHelper::getDoubleFromQML("init_z_text", "text");
    // Считанные значения имеют тип double (8 байт), специальный метод разбивает их на байты
    // и добавляет в пакет.
    QByteArrayHelper::appendAndStuff(cmd, initX);
    QByteArrayHelper::appendAndStuff(cmd, initY);
    QByteArrayHelper::appendAndStuff(cmd, initZ);

    qDebug() << *cmd;
}

// См. комментарий выше.
void CommandBuilder::build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *cmd)
{
    double initLat = QMLDataHelper::getDoubleFromQML("init_lat_text", "text");
    double initLong = QMLDataHelper::getDoubleFromQML("init_long_text", "text");
    double initAlt = QMLDataHelper::getDoubleFromQML("init_alt_text", "text");
    // Считанные значения имеют тип double (8 байт), специальный метод разбивает их на байты
    // и добавляет в пакет.
    QByteArrayHelper::appendAndStuff(cmd, initLat);
    QByteArrayHelper::appendAndStuff(cmd, initLong);
    QByteArrayHelper::appendAndStuff(cmd, initAlt);

    qDebug() << *cmd;
}

// Данный метод собирает пакет 0х38. Первый байт данных - "1" - добавлен ранее как подкод.
// Далее нужно добавить код информации, которую желаем получить (он равен номеру выбранного пункта
// в выпадающем списке в интерфейсе плюс 2) и номер спутника. Номер спутника нужен не для всех типов
// данных, однако поле, тем не менее, нужно чем-то заполнить.
void CommandBuilder::build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *cmd)
{
    // Этот ComboBox - на вкладке "Информация о спутниках".
    quint8 typeOfData = (quint8)(QMLDataHelper::getIntFromQML("typeOfDataComboBox", "currentIndex") + 2);

    quint8 satelliteIndex = (quint8)QMLDataHelper::getIntFromQML("satellite_selection_spinner1", "value");
    QByteArrayHelper::appendAndStuff(cmd, typeOfData);
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

// Этот метод делает всего одну вещь - получает номер спутника, которому нужно включить/отключить отслеживание
// здоровья или включение его в список разрешенных спутников GPS-модуля. Тип операции уже был занесен в пакет ранее.
void CommandBuilder::build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *cmd)
{
    quint8 satelliteIndex = (quint8)QMLDataHelper::getIntFromQML("satellites_and_health_spinner", "value");
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

// Еще один метод для получения вкл/выкл опций из переключателей на вкладке "Настройка авторассылки"
// и превращения этих опций в битовое поле.
void CommandBuilder::build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *cmd)
{
    bool maskPrimaryPackets = QMLDataHelper::getBoolFromQML("primaryPacketMaskingBit", "checked");
    bool maskSupplPackets = QMLDataHelper::getBoolFromQML("supplPacketMaskingBit", "checked");
    bool maskOtherPackets = QMLDataHelper::getBoolFromQML("otherPacketsMaskingBit", "checked");

    // Создание битового поля.
    quint8 infoByte = (quint8)(
                (maskPrimaryPackets ? BIT0 : ZERO_BYTE) |
                (maskSupplPackets ? BIT2 : ZERO_BYTE) |
                (maskOtherPackets ? BIT6 : ZERO_BYTE)
               );

    // В документации написано, что нужно поместить битовое поле в первый байт, а остальные рарезервированы.
    // Тесты же показали, что первый байт ни на что не влияет. Так или иначе, отправка поля во всех четырех байтах
    // дает желаемый результат (возможно, виноват порядок байт).
    QByteArrayHelper::appendAndStuff(cmd, infoByte);
    QByteArrayHelper::appendAndStuff(cmd, infoByte);
    QByteArrayHelper::appendAndStuff(cmd, infoByte);
    QByteArrayHelper::appendAndStuff(cmd, infoByte);

    qDebug() << *cmd;
}

// Метод забирает из кода интерфейса QML значение специальной переменной, которая меняется при нажатии
// одной из кнопок на вкладке "Пакеты по таймингу", и ее значение после нажатия определяет тип запроса, который нужно отправить.
void CommandBuilder::build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *cmd)
{
    quint8 typeOfRequest = (quint8)QMLDataHelper::getIntFromQML("timingPacketItem", "timingPacketType");
    // Просто добавим этот тип запроса в пакет.
    QByteArrayHelper::appendAndStuff(cmd, typeOfRequest);

    qDebug() << *cmd;
}
