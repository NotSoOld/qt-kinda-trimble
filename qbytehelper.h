#ifndef QBYTEHELPER_H
#define QBYTEHELPER_H

#include <QObject>
#include "kinda_trimble_shared.h"
#include "converter.h"

class QByteHelper : public QObject
{
    Q_OBJECT
public:
    //explicit QByteHelper(QObject *parent = nullptr);

    static void appendAndStuff(QByteArray *, byte);
    static void appendAndStuff(QByteArray *, double);
    static void appendAndStuff(QByteArray *, float);
    static void appendAndStuff(QByteArray *, unsigned short);
    static void appendAndStuff(QByteArray *, unsigned int);
signals:

public slots:
};

#endif // QBYTEHELPER_H