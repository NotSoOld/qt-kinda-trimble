#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickStyle>
#include "comhandler.h"
#include "qmldatahelper.h"

COMHandler handler;
COMHandler receiverThread;
//QQuickWindow *window;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QMLDataHelper::mainWindow = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
    if (!QMLDataHelper::mainWindow) {
        qDebug() << QString("Не удается найти основное окно");
        exit(1);
    }
    //handler.window = window;
    //receiverThread.window = window;

    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_send_command(int, int)),
                &handler,
                SLOT(send_command(int, int))
    );

    QObject::connect(
                &receiverThread,
                SIGNAL(appendReceivedText(QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onAppendReceivedtext(QVariant))
    );

    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_open_port(QString, int, int, int, int, int)),
                &receiverThread,
                SLOT(configureCOM(QString, int, int, int, int, int))
    );

    QObject::connect(
                &receiverThread,
                SIGNAL(newValuesGained(QVariant, QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onGainNewValues(QVariant, QVariant))
    );

   // receiverThread.methodToStartThreadWith = &COMHandler::receiveReport;
   // receiverThread.start();

    app.exec();
    receiverThread.terminate();
    COMHandler::finishCOM();
    return 0;
}
