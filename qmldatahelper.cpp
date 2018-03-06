#include "qmldatahelper.h"

QQuickWindow *QMLDataHelper::mainWindow;

bool QMLDataHelper::getBoolFromQML(const char *qmlName, const char *propertyName)
{
    QObject *qmlObject = mainWindow->findChild<QObject *>(qmlName);
    if (!qmlObject) {
        qDebug() << QString("Не удается найти QML-компонент %0").arg(qmlName);
        exit(1);
    }
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