#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include "kinda_trimble_shared.h"
#include <QQuickView>
#include "comhandler.h"

class PacketParser : public QObject
{
    Q_OBJECT
public:
    PacketParser(QByteArray);
    quint8 reportCode();
    quint8 reportSubcode();
    QString analyseAndParse();

private:
    QString parse_REPORT_UNPARSABLE();
    QString parse_RPTSUB_FIRMWARE_VERSION();
    QString parse_RPTSUB_HARDWARE_COMPONENT_INFO();
    QString parse_REPORT_DOUBLE_XYZ_POS(/*COMHandler **/);
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
/*
    struct RPTSUB_FIRMWARE_VERSION_reportStruct {

        quint8 reportSubcode;
        quint8 reserved;
        quint8 majorVersion;
        quint8 minorVersion;
        quint8 buildNumber;
        quint8 month;
        quint8 day;
        quint16 year;
        quint8 productNameLength;
        QString productName;
    } RPTSUB_FIRMWARE_VERSION_report;

    struct RPTSUB_SUPPL_TIMING_PACKET_reportStruct {

        quint8 reportSubcode;
        quint8 receiverMode;
        quint8 discipliningMode;
        quint8 selfSurveyProgress;
        quint32 holdoverDuration;
        quint16 criticalAlarmsBF;
        quint16 minorAlarmsBF;
        quint8 gpsDecodingStatus;
        quint8 discipliningActivity;
        quint8 spare1;
        quint8 spare2;
        float ppsOffset;
        float clockOffset;
        quint32 dacValue;
        float dacVoltage;
        float temperature;
        double latitude;
        double longitude;
        double altitude;
        float ppsQuantizationError;
        quint8 spare3;
        quint8 spare4;
        quint8 spare5;
    } RPTSUB_SUPPL_TIMING_PACKET_report;
*/
    //void updateInterfaceValues();

    QByteArray data;
    quint8 _reportCode;

};

#endif // PACKETPARSER_H