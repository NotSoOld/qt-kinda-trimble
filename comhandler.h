#ifndef COMHANDLER_H
#define COMHANDLER_H

#include "kinda_trimble_shared.h"

#include <QSerialPort>
#include <QThread>
#include "converter.h"
#include "packetparser.h"
#include "qbytehelper.h"
#include "qmldatahelper.h"

class COMHandler : public QThread
{
    Q_OBJECT
public:
    static void finishCOM();
    void receiveReport();
    void run();

    void build_COMMAND_SET_IO_OPTIONS(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_XYZ(QByteArray *);
    void build_COMMAND_ACCURATE_INIT_POS_LLA(QByteArray *);
    void build_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA(QByteArray *);
    void build_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH(QByteArray *);
    void build_CMDSUB_SET_PACKET_BROADCAST_MASK(QByteArray *);
    void build_CMDSUB_REQUEST_TIMING_PACKET(QByteArray *);



    void (COMHandler::*methodToStartThreadWith)();
    static QString name;
    static QSerialPort *com;
    static QByteArray readedData;
    static byte previouslyReadedChar;
    QQuickWindow *window;

signals:
    void appendReceivedText(QVariant s);
public slots:
    void send_command(int, int);
    void readFromCOM();
    void configureCOM(QString, int, int, int, int, int);
};

#endif // COMHANDLER_H
