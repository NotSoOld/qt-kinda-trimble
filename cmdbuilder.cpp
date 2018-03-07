#include "cmdbuilder.h"

void CommandBuilder::build_COMMAND_SET_IO_OPTIONS(QByteArray *cmd)
{
    bool ecefChecked = QMLDataHelper::getBoolFromQML("eCEFcheck", "checked");
    bool llaChecked = QMLDataHelper::getBoolFromQML("lLAcheck", "checked");
    bool precision = QMLDataHelper::getBoolFromQML("doublePrecRB", "checked");
    bool gpsTime = QMLDataHelper::getBoolFromQML("gpsTimeRB", "checked");
    bool mslChecked = QMLDataHelper::getBoolFromQML("mslRB", "checked");
    bool ecefVelChecked = QMLDataHelper::getBoolFromQML("ecef_vel_check", "checked");
    bool enuVelChecked = QMLDataHelper::getBoolFromQML("enu_vel_check", "checked");
    bool raw_data_report_checked = QMLDataHelper::getBoolFromQML("raw_data_report_check", "checked");
    bool dbhz_out_checked = QMLDataHelper::getBoolFromQML("dbhz_out_RB", "checked");

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

void CommandBuilder::build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *cmd)
{
    double initX = QMLDataHelper::getDoubleFromQML("init_x_text", "text");
    double initY = QMLDataHelper::getDoubleFromQML("init_y_text", "text");
    double initZ = QMLDataHelper::getDoubleFromQML("init_z_text", "text");
    QByteArrayHelper::appendAndStuff(cmd, initX);
    QByteArrayHelper::appendAndStuff(cmd, initY);
    QByteArrayHelper::appendAndStuff(cmd, initZ);

    qDebug() << *cmd;
}

void CommandBuilder::build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *cmd)
{
    double initLat = QMLDataHelper::getDoubleFromQML("init_lat_text", "text");
    double initLong = QMLDataHelper::getDoubleFromQML("init_long_text", "text");
    double initAlt = QMLDataHelper::getDoubleFromQML("init_alt_text", "text");
    QByteArrayHelper::appendAndStuff(cmd, initLat);
    QByteArrayHelper::appendAndStuff(cmd, initLong);
    QByteArrayHelper::appendAndStuff(cmd, initAlt);

    qDebug() << *cmd;
}

void CommandBuilder::build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *cmd)
{
    quint8 typeOfData = (quint8)(QMLDataHelper::getIntFromQML("typeOfDataComboBox", "currentIndex") + 2);
    quint8 satelliteIndex = (quint8)QMLDataHelper::getIntFromQML("satellite_selection_spinner1", "value");
    QByteArrayHelper::appendAndStuff(cmd, typeOfData);
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void CommandBuilder::build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *cmd)
{
    quint8 satelliteIndex = (quint8)QMLDataHelper::getIntFromQML("satellites_and_health_spinner", "value");
    QByteArrayHelper::appendAndStuff(cmd, satelliteIndex);

    qDebug() << *cmd;
}

void CommandBuilder::build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *cmd)
{
    bool maskPrimaryPackets = QMLDataHelper::getBoolFromQML("primaryPacketMaskingBit", "checked");
    bool maskSupplPackets = QMLDataHelper::getBoolFromQML("supplPacketMaskingBit", "checked");
    bool maskOtherPackets = QMLDataHelper::getBoolFromQML("otherPacketsMaskingBit", "checked");
    QByteArrayHelper::appendAndStuff(cmd, (quint8)(
                       (maskPrimaryPackets ? BIT0 : ZERO_BYTE) |
                       (maskSupplPackets ? BIT2 : ZERO_BYTE) |
                       (maskOtherPackets ? BIT6 : ZERO_BYTE)
                      ));

    QByteArrayHelper::appendAndStuff(cmd, ZERO_BYTE);
    QByteArrayHelper::appendAndStuff(cmd, ZERO_BYTE);
    QByteArrayHelper::appendAndStuff(cmd, ZERO_BYTE);

    qDebug() << *cmd;
}

void CommandBuilder::build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *cmd)
{
    quint8 typeOfRequest = (quint8)QMLDataHelper::getIntFromQML("timingPacketItem", "timingPacketType");
    QByteArrayHelper::appendAndStuff(cmd, typeOfRequest);

    qDebug() << *cmd;
}
