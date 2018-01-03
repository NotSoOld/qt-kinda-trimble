#ifndef COMHANDLER_H
#define COMHANDLER_H

#include <QObject>
#include <QtCore>
#include <QSerialPort>
#include <QThread>
#include <QQuickWindow>
#include <QDebug>

#define PI 3.1415926535898
#define DLE 0x10
#define ETX 0x03
#define BIT(x) (1 << (7 - x))

#define COMMAND_FIRMWARE_INFO                           0x1C
#define     CMDSUB_FIRMWARE_VERSION                     0x01
#define     CMDSUB_HARDWARE_COMPONENT_INFO              0x03
#define COMMAND_INITIATE_RESET                          0x1E
#define COMMAND_REQEST_SOFTWARE_VERSION                 0x1F
#define COMMAND_REQUEST_GPS_SATELLITES                  0x24
#define COMMAND_INITIATE_HOT_RESET                      0x25
#define COMMAND_REQUEST_SIGNAL_LEVELS                   0x27
#define COMMAND_ACCURATE_INIT_POS_XYZ                   0x31
#define COMMAND_ACCURATE_INIT_POS_LLA                   0x32
#define COMMAND_SATELLITE_SELECTION                     0x34
#define COMMAND_SET_IO_OPTIONS                          0x35
#define COMMAND_REQUEST_STATUS_AND_POS                  0x37
#define COMMAND_REQUEST_SATELLITE_SYSTEM_DATA           0x38
#define COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH       0x39
#define COMMAND_REQUEST_LAST_RAW_MEASUREMENT            0x3A
#define COMMAND_REQUEST_SATELLITE_TRACKING_STATUS       0x3C
#define COMMAND_SET_RECEIVER_CONFIG                     0xBB
#define COMMAND_SET_PORT_CONFIG                         0xBC
#define COMMAND_SUPER                                   0x8E
#define     CMDSUB_REQUEST_CURRENT_DATUM                0x15
#define     CMDSUB_WRITE_CONFIG_TO_FLASH                0x26
#define     CMDSUB_REQUEST_MANUFACT_PARAMS              0x41
#define     CMDSUB_REQUEST_STORED_PRODUCTION_PARAMS     0x42
#define     CMDSUB_SET_PPS_CHARS                        0x4A
#define     CMDSUB_SET_PPS_OUTPUT                       0x4E
#define     CMDSUB_SET_DAC                              0xA0
#define     CMDSUB_SET_UTC_GPS_TIMIMG                   0xA2
#define     CMDSUB_ISSUE_OSC_DISCIPL_CMD                0xA3
#define     CMDSUB_TEST_MODES                           0xA4
#define     CMDSUB_SET_PACKET_BROADCAST_MASK            0xA5
#define     CMDSUB_ISSUE_SELF_SURVEY                    0xA6
#define     CMDSUB_SET_REQUEST_DISCIPL_PARAMS           0xA8
#define     CMDSUB_SET_SELF_SURVEY_PARAMS               0xA9
#define     CMDSUB_REQUEST_PRIMARY_TIMING_PACKET        0xAB
#define     CMDSUB_REQUEST_SUPPL_TIMING_PACKET          0xAC

#define REPORT_UNPARSABLE                               0x13
#define REPORT_FIRMWARE_INFO                            0x1C
#define     RPTSUB_FIRMWARE_VERSION                     0x81
#define     RPTSUB_HARDWARE_COMPONENT_INFO              0x83
#define REPORT_SINGLE_XYZ_POS                           0x42
#define REPORT_SINGLE_VELOCITY_FIX_XYZ                  0x43
#define REPORT_SOFTWARE_VERSION_INFO                    0x45
#define REPORT_TRACKED_SATELLITES_SINGAL_LVL            0x47
#define REPORT_SINGLE_LLA_POS                           0x4A
#define REPORT_SINGLE_VELOCITY_FIX_ENU                  0x56
#define REPORT_LAST_FIX_INFO                            0x57
#define REPORT_GPS_SYSTEM_DATA                          0x58
#define REPORT_STATUS_SATELLITE_HEALTH                  0x59
#define REPORT_RAW_MEASUREMENT_DATA                     0x5A
#define REPORT_SATELLITE_TRACKING_STATUS                0x5C
#define REPORT_SATELLITE_SELECTION_LIST                 0x6D
#define REPORT_DOUBLE_XYZ_POS                           0x83
#define REPORT_DOUBLE_LLA_POS                           0x84
#define REPORT_SUPER                                    0x8F
#define     RPTSUB_CURRENT_DATUM                        0x15
#define     RPTSUB_MANUFACT_PARAMS                      0x41
#define     RPTSUB_STORED_PRODUCTION_PARAMS             0x42
#define     RPTSUB_PPS_CHARS                            0x4A
#define     RPTSUB_PPS_OUTPUT                           0x4E
#define     RPTSUB_DAC                                  0xA0
#define     RPTSUB_UTC_GPS_TIMIMG                       0xA2
#define     RPTSUB_OSC_DISCIPL_CMD                      0xA3
#define     RPTSUB_TEST_MODES                           0xA4
#define     RPTSUB_PACKET_BROADCAST_MASK                0xA5
#define     RPTSUB_SELF_SURVEY                          0xA6
#define     RPTSUB_DISCIPL_PARAMS                       0xA8
#define     RPTSUB_SELF_SURVEY_PARAMS                   0xA9
#define     RPTSUB_PRIMARY_TIMING_PACKET                0xAB
#define     RPTSUB_SUPPL_TIMING_PACKET                  0xAC

enum ParserStatus {
    Waiting_for_message,
    Reading_data,
    Found_DLE
};


class COMHandler : public QThread
{
    Q_OBJECT
public:
    void configureCOM(QString, QIODevice::OpenModeFlag);
    void finishCOM();
    void receiveText();
    void run();

    void build_COMMAND_SET_IO_OPTIONS(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *);
    void build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *);

    QString parse_REPORT_UNPARSABLE(QByteArray);
    QString parse_REPORT_DOUBLE_XYZ_POS(QByteArray);
    QString parse_REPORT_DOUBLE_LLA_POS(QByteArray);

    void append(QByteArray *, byte);
    void append(QByteArray *, double);
    void append(QByteArray *, float);
    void append(QByteArray *, unsigned short);
    void append(QByteArray *, unsigned int);

    bool getBoolFromQML(const char *, const char *);
    int getIntFromQML(const char *, const char *);
    double getDoubleFromQML(const char *, const char *);

    void (COMHandler::*methodToStartThreadWith)();
    QString name;
    QSerialPort *com;
    ParserStatus parser;
    QQuickWindow *window;

signals:
    void appendReceivedText(QVariant s);
public slots:
    void send_command(int, int);
};

#endif // COMHANDLER_H
