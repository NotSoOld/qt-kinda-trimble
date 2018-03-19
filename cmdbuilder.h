#ifndef CMDBUILDER_H
#define CMDBUILDER_H

#include "kinda_trimble_shared.h"
#include "qmldatahelper.h"
#include "qbytehelper.h"

// Класс CommandBuilder содержит в себе все методы для сборки настраиваемых пакетов перед отправкой.
// Если код и подкод помещаются в пакет до вызова этих методов (иногда вместо подкода
// вставляется первый байт пакета, об этом будет предупреждено в комментариях), то
// остальные данные из интерфейса QML добавляются именно методами этого класса.
// Каждый метод написан отдельно под каждый пакет.
class CommandBuilder : public QObject
{
    Q_OBJECT
public:
    static void build_COMMAND_SET_IO_OPTIONS(QByteArray *);
    static void build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *);
    static void build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *);
    static void build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *);
    static void build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *);
    static void build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *);
    static void build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *);

};

#endif // CMDBUILDER_H