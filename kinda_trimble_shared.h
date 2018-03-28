#ifndef KINDA_TRIMBLE_SHARED_H
#define KINDA_TRIMBLE_SHARED_H

// Содержит всё, что необходимо остальным файлам .h и .cpp: include'ы, константы и макросы.

#include <QObject>
#include <QtCore>
#include <QQuickWindow>
#include <QDebug>

// Константы.
#define PI ((double)3.1415926535898)
#define DLE ((quint8)0x10)
#define ETX ((quint8)0x03)
#define ZERO_BYTE ((quint8)0)
// Похоже, что используется именно такое именование битов (bit numbering).
#define BIT0 ((quint8)1)
#define BIT1 ((quint8)2)
#define BIT2 ((quint8)4)
#define BIT3 ((quint8)8)
#define BIT4 ((quint8)16)
#define BIT5 ((quint8)32)
#define BIT6 ((quint8)64)
#define BIT7 ((quint8)128)

// Байты кодов и подкодов пакетов команд и запросов.
// (Некоторые закомментированы, потому что эти пакеты не пригодились или не были реализованы.)
#define COMMAND_FIRMWARE_INFO                           ((quint8)0x1C)
#define     CMDSUB_FIRMWARE_VERSION                     ((quint8)0x01)
#define     CMDSUB_HARDWARE_COMPONENT_INFO              ((quint8)0x03)
#define COMMAND_INITIATE_RESET                          ((quint8)0x1E)
#define COMMAND_REQEST_SOFTWARE_VERSION                 ((quint8)0x1F)
#define COMMAND_REQUEST_GPS_SATELLITES                  ((quint8)0x24)
#define COMMAND_INITIATE_HOT_RESET                      ((quint8)0x25)
#define COMMAND_REQUEST_SIGNAL_LEVELS                   ((quint8)0x27)
#define COMMAND_ACCURATE_INIT_POS_XYZ                   ((quint8)0x31)
#define COMMAND_ACCURATE_INIT_POS_LLA                   ((quint8)0x32)
#define COMMAND_SATELLITE_SELECTION                     ((quint8)0x34)
#define COMMAND_SET_IO_OPTIONS                          ((quint8)0x35)
#define COMMAND_REQUEST_STATUS_AND_POS                  ((quint8)0x37)
#define COMMAND_REQUEST_SATELLITE_SYSTEM_DATA           ((quint8)0x38)
#define COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH       ((quint8)0x39)
#define COMMAND_REQUEST_LAST_RAW_MEASUREMENT            ((quint8)0x3A)
#define COMMAND_REQUEST_SATELLITE_TRACKING_STATUS       ((quint8)0x3C)
#define COMMAND_SET_RECEIVER_CONFIG                     ((quint8)0xBB)
#define COMMAND_SET_PORT_CONFIG                         ((quint8)0xBC)
#define COMMAND_SUPER                                   ((quint8)0x8E)
/*#define     CMDSUB_REQUEST_CURRENT_DATUM                ((quint8)0x15)
#define     CMDSUB_WRITE_CONFIG_TO_FLASH                ((quint8)0x26)
#define     CMDSUB_REQUEST_MANUFACT_PARAMS              ((quint8)0x41)
#define     CMDSUB_REQUEST_STORED_PRODUCTION_PARAMS     ((quint8)0x42)
#define     CMDSUB_SET_PPS_CHARS                        ((quint8)0x4A)
#define     CMDSUB_SET_PPS_OUTPUT                       ((quint8)0x4E)
#define     CMDSUB_SET_DAC                              ((quint8)0xA0)
#define     CMDSUB_SET_UTC_GPS_TIMIMG                   ((quint8)0xA2)
#define     CMDSUB_ISSUE_OSC_DISCIPL_CMD                ((quint8)0xA3)
#define     CMDSUB_TEST_MODES                           ((quint8)0xA4)*/
#define     CMDSUB_SET_PACKET_BROADCAST_MASK            ((quint8)0xA5)
/*#define     CMDSUB_ISSUE_SELF_SURVEY                    ((quint8)0xA6)
#define     CMDSUB_SET_REQUEST_DISCIPL_PARAMS           ((quint8)0xA8)
#define     CMDSUB_SET_SELF_SURVEY_PARAMS               ((quint8)0xA9)*/
#define     CMDSUB_REQUEST_PRIMARY_TIMING_PACKET        ((quint8)0xAB)
#define     CMDSUB_REQUEST_SUPPL_TIMING_PACKET          ((quint8)0xAC)

#define REPORT_UNPARSABLE                               ((quint8)0x13)
#define REPORT_FIRMWARE_INFO                            ((quint8)0x1C)
#define     RPTSUB_FIRMWARE_VERSION                     ((quint8)0x81)
#define     RPTSUB_HARDWARE_COMPONENT_INFO              ((quint8)0x83)
#define REPORT_SINGLE_XYZ_POS                           ((quint8)0x42)
#define REPORT_SINGLE_VELOCITY_FIX_XYZ                  ((quint8)0x43)
#define REPORT_SOFTWARE_VERSION_INFO                    ((quint8)0x45)
#define REPORT_TRACKED_SATELLITES_SINGAL_LVL            ((quint8)0x47)
#define REPORT_SINGLE_LLA_POS                           ((quint8)0x4A)
#define REPORT_REQUEST_IO_OPTIONS                       ((quint8)0x55)
#define REPORT_SINGLE_VELOCITY_FIX_ENU                  ((quint8)0x56)
#define REPORT_LAST_FIX_INFO                            ((quint8)0x57)
#define REPORT_GPS_SYSTEM_DATA                          ((quint8)0x58)
#define REPORT_STATUS_SATELLITE_HEALTH                  ((quint8)0x59)
#define REPORT_RAW_MEASUREMENT_DATA                     ((quint8)0x5A)
#define REPORT_SATELLITE_TRACKING_STATUS                ((quint8)0x5C)
#define REPORT_SATELLITE_SELECTION_LIST                 ((quint8)0x6D)
#define REPORT_DOUBLE_XYZ_POS                           ((quint8)0x83)
#define REPORT_DOUBLE_LLA_POS                           ((quint8)0x84)
#define REPORT_SUPER                                    ((quint8)0x8F)
/*#define     RPTSUB_CURRENT_DATUM                        ((quint8)0x15)
#define     RPTSUB_MANUFACT_PARAMS                      ((quint8)0x41)
#define     RPTSUB_STORED_PRODUCTION_PARAMS             ((quint8)0x42)
#define     RPTSUB_PPS_CHARS                            ((quint8)0x4A)
#define     RPTSUB_PPS_OUTPUT                           ((quint8)0x4E)
#define     RPTSUB_DAC                                  ((quint8)0xA0)
#define     RPTSUB_UTC_GPS_TIMIMG                       ((quint8)0xA2)
#define     RPTSUB_OSC_DISCIPL_CMD                      ((quint8)0xA3)
#define     RPTSUB_TEST_MODES                           ((quint8)0xA4)*/
#define     RPTSUB_PACKET_BROADCAST_MASK                ((quint8)0xA5)
/*#define     RPTSUB_SELF_SURVEY                          ((quint8)0xA6)
#define     RPTSUB_DISCIPL_PARAMS                       ((quint8)0xA8)
#define     RPTSUB_SELF_SURVEY_PARAMS                   ((quint8)0xA9)*/
#define     RPTSUB_PRIMARY_TIMING_PACKET                ((quint8)0xAB)
#define     RPTSUB_SUPPL_TIMING_PACKET                  ((quint8)0xAC)

#endif // KINDA_TRIMBLE_SHARED_H
