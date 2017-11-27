#ifndef COMHANDLER_H
#define COMHANDLER_H

#include <QObject>
#include <QtCore>
#include <QSerialPort>
#include <QThread>
#include <QQuickWindow>
#include <QDebug>

#define DLE 0x10
#define ETX 0x03
#define COMMAND_SET_IO_OPTIONS 0x35
#define COMMAND_STATUS_AND_POS 0x37
#define BIT(x) (1 << (7 - x))
#define REPORT_DOUBLE_XYZ_POS 0x83
#define REPORT_DOUBLE_LLA_POS 0x84

enum ParserStatus {
    Waiting_for_message,
    Reading_data,
    Found_DLE
};

union __double {
    double value;
    unsigned char bytes[8];
};

union __single {
    float value;
    unsigned char bytes[4];
};


class COMHandler : public QThread
{
    Q_OBJECT
public:
    void configureCOM(QString, QIODevice::OpenModeFlag);
    void finishCOM();
    void receiveText();
    void run();
    float bytesToSingle(QByteArray, int);
    double bytesToDouble(QByteArray, int);
    QString parseDoubleXYZPos(QByteArray);
    QString parseDoubleLLAPos(QByteArray);
    void (COMHandler::*methodToStartThreadWith)();
    QString name;
    QSerialPort *com;
    ParserStatus parser;
    QQuickWindow *window;

signals:
    void appendReceivedText(QVariant s);
public slots:
    void sendCommandPos();
    void sendCommandIOOptions();
};

#endif // COMHANDLER_H
