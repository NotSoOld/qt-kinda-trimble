#include "qmldatahelper.h"

/*
QMLDataObtainer::QMLDataObtainer(QObject *parent) : QObject(parent)
{

}
*/

bool QMLDataHelper::getBoolFromQML(QQuickWindow *window, const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toBool();
}

int QMLDataHelper::getIntFromQML(QQuickWindow *window, const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toInt();
}

double QMLDataHelper::getDoubleFromQML(QQuickWindow *window, const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = window->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
    return qmlObject->property(propertyName).toDouble();
}