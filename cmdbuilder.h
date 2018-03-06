#ifndef CMDBUILDER_H
#define CMDBUILDER_H

#include "kinda_trimble_shared.h"
#include "qmldatahelper.h"
#include "qbytehelper.h"

class CommandBuilder : public QObject
{
    Q_OBJECT
public:
    void build_COMMAND_SET_IO_OPTIONS(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *);
    void build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *);
    void build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *);
    void build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *);
    void build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *);

signals:

public slots:
};

#endif // CMDBUILDER_H