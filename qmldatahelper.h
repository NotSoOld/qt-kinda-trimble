#ifndef QMLDATAOBTAINER_H
#define QMLDATAOBTAINER_H

#include "kinda_trimble_shared.h"

class QMLDataHelper : public QObject
{
    Q_OBJECT
public:
    // Находится здесь для всех классов, т.к. периодически тому или иному методу
    // нужен доступ к главному окну программы.
    static QQuickWindow *mainWindow;

    static bool getBoolFromQML(const char *, const char *);
    static int getIntFromQML(const char *, const char *);
    static double getDoubleFromQML(const char *, const char *);
signals:

public slots:
};

#endif // QMLDATAOBTAINER_H