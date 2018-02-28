#ifndef QMLDATAOBTAINER_H
#define QMLDATAOBTAINER_H

#include "kinda_trimble_shared.h"

class QMLDataHelper : public QObject
{
    Q_OBJECT
public:
    //explicit QMLDataObtainer(QObject *parent = nullptr);

    static bool getBoolFromQML(QQuickWindow *, const char *, const char *);
    static int getIntFromQML(QQuickWindow *, const char *, const char *);
    static double getDoubleFromQML(QQuickWindow *, const char *, const char *);
signals:

public slots:
};

#endif // QMLDATAOBTAINER_H