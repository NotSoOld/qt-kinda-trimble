#ifndef QBYTEHELPER_H
#define QBYTEHELPER_H

#include "kinda_trimble_shared.h"
#include "converter.h"

class QByteArrayHelper : public QObject
{
    Q_OBJECT
public:
    static void appendAndStuff(QByteArray *, quint8);
    static void appendAndStuff(QByteArray *, double);
    static void appendAndStuff(QByteArray *, float);
    static void appendAndStuff(QByteArray *, quint16);
    static void appendAndStuff(QByteArray *, quint32);
signals:

public slots:
};

#endif // QBYTEHELPER_H