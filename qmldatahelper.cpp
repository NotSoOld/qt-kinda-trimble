#include "qmldatahelper.h"

QQuickWindow *QMLDataHelper::mainWindow;

// Все методы работают аналогичным образом.

bool QMLDataHelper::getBoolFromQML(const char *qmlName, const char *propertyName)
{
    // Сначала находим элемент QML по оъектному имени (а если не найден - сообщаем об ошибке).
    QObject *qmlObject = mainWindow->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    // Потом получаем значение свойства по его имени.
    return qmlObject->property(propertyName).toBool();
}

int QMLDataHelper::getIntFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = mainWindow->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toInt();
}

double QMLDataHelper::getDoubleFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = mainWindow->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toDouble();
}