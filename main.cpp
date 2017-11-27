#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <comhandler.h>

COMHandler handler;
COMHandler receiverThread;
QQuickWindow *window;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    handler.configureCOM("COM3", QIODevice::ReadWrite);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    window = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
    handler.window = window;
    QObject::connect(
                window,
                SIGNAL(sigSendCommandPos()),
                &handler,
                SLOT(sendCommandPos())
    );
    QObject::connect(
                window,
                SIGNAL(sigSendCommandIOOptions()),
                &handler,
                SLOT(sendCommandIOOptions())
    );
    QObject::connect(
                &receiverThread,
                SIGNAL(appendReceivedText(QVariant)),
                window,
                SLOT(onAppendReceivedtext(QVariant))
    );
    receiverThread.configureCOM("COM4", QIODevice::ReadWrite);
    receiverThread.window = window;
    receiverThread.methodToStartThreadWith = &COMHandler::receiveText;
    receiverThread.start();

    app.exec();
    receiverThread.terminate();
    handler.finishCOM();
    receiverThread.finishCOM();
    return 0;
}
