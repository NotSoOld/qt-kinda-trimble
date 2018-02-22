#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <converter.h>
#include <comhandler.h>

class PacketParser : public QObject
{
    Q_OBJECT
public:
    PacketParser(QByteArray);
    byte reportCode();
    byte reportSubcode();

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
    QString parse_REPORT_GPS_SYSTEM_DATA();
    QString parse_REPORT_STATUS_SATELLITE_HEALTH();
    QString parse_REPORT_RAW_MEASUREMENT_DATA();
    QString parse_REPORT_SATELLITE_TRACKING_STATUS();
    QString parse_REPORT_SATELLITE_SELECTION_LIST();
    QString parse_RPTSUB_PRIMARY_TIMING_PACKET();
    QString parse_RPTSUB_SUPPL_TIMING_PACKET();
private:
    QByteArray data;

signals:

public slots:
};

#endif // PACKETPARSER_H