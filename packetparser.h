#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include "kinda_trimble_shared.h"
#include <QQuickView>
#include "comhandler.h"

// Данный класс полностью отвечает за преобразование пакетов в удобочитаемые данные,
// отображающиеся либо в интерфейсе, либо в области логгирования.
class PacketParser : public QObject
{
    Q_OBJECT
public:
    // Для использования возможностей данного класса необходимо создать объект.
    // Он получит пакет в качестве параметра, который будет храниться в закрытой переменной.
    PacketParser(QByteArray);
    // Акцессоры для получения кода и подкода хранимого пакета.
    quint8 reportCode();
    quint8 reportSubcode();
    // Метод, запускающий разбор пакета, который был передан объекту. Только этот функциональный метод
    // открыт для использования извне, остальные методы разбора закрыты, т.к. только этот метод
    // может безошибочно выбрать нужный метод для разбора (инкапсуляция того, что не предназначено
    // для "чужих глаз").
    QString analyseAndParse();

    // Все закрытые методы ниже работают по схожему принципу - они разбирают хранящийся в объекте
    // пакет байт за байтом, байты преобразуются согласно структуре пакета, приведенной в документации
    // для каждого пакета отдельно. Метод analyseAndParse() сам выберет нужный метод разбора из многих.
    // В качестве возвращаемой строки вернется строка лога пакета, а все важные величины будут
    // автоматически отправлены в интерфейс внутри метода разбора (самим методом).
    // Имя метода без "parse_" соответствует макросу - коду/подкоду разбираемого им пакета.
private:
    QString parse_REPORT_UNPARSABLE();
    QString parse_RPTSUB_FIRMWARE_VERSION();
    QString parse_RPTSUB_HARDWARE_COMPONENT_INFO();
    QString parse_REPORT_DOUBLE_XYZ_POS();
    QString parse_REPORT_DOUBLE_LLA_POS();
    QString parse_REPORT_SINGLE_XYZ_POS();
    QString parse_REPORT_SINGLE_VELOCITY_FIX_XYZ();
    QString parse_REPORT_SOFTWARE_VERSION_INFO();
    QString parse_REPORT_TRACKED_SATELLITES_SINGAL_LVL();
    QString parse_REPORT_SINGLE_LLA_POS();
    QString parse_REPORT_REQUEST_IO_OPTIONS();
    QString parse_REPORT_SINGLE_VELOCITY_FIX_ENU();
    QString parse_REPORT_LAST_FIX_INFO();
    QString parse_REPORT_GPS_SYSTEM_DATA();
    QString parse_REPORT_STATUS_SATELLITE_HEALTH();
    QString parse_REPORT_RAW_MEASUREMENT_DATA();
    QString parse_REPORT_SATELLITE_TRACKING_STATUS();
    QString parse_REPORT_SATELLITE_SELECTION_LIST();
    QString parse_RPTSUB_PRIMARY_TIMING_PACKET();
    QString parse_RPTSUB_SUPPL_TIMING_PACKET();
    QString parse_RPTSUB_PACKET_BROADCAST_MASK();

    // Внутренний массив, содержит пакет полностью в виде последовательности байтов.
    QByteArray data;
    // Код пакета, в массиве выше его нет. Подкод - нулевой байт массива.
    quint8 _reportCode;

};

#endif // PACKETPARSER_H