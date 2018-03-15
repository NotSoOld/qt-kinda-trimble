#ifndef QMLDATAOBTAINER_H
#define QMLDATAOBTAINER_H

#include "kinda_trimble_shared.h"

// Класс-помощник для быстрого получения значений свойств элементов интерфейса QML.
class QMLDataHelper : public QObject
{
    Q_OBJECT
public:
    // Находится здесь для всех классов, т.к. периодически тому или иному методу
    // нужен доступ к главному окну программы (плюс он статический член класса, проще обращаться).
    static QQuickWindow *mainWindow;

    // Методы для быстрого доступа к свойствам элементов: первым указывается объектное имя элемента, вторым - имя свойства.
    // Для каждого типа, возвращаемого свойством - отдельный метод.
    static bool getBoolFromQML(const char *, const char *);
    static int getIntFromQML(const char *, const char *);
    static double getDoubleFromQML(const char *, const char *);

};

#endif // QMLDATAOBTAINER_H