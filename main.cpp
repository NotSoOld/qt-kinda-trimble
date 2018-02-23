#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickStyle>
#include <comhandler.h>

COMHandler handler;
COMHandler receiverThread;
QQuickWindow *window;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");

    COMHandler::configureCOM("COM5", QIODevice::ReadWrite);

    //engine.load(QUrl(QStringLiteral("qrc:/COMInit.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    window = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
    handler.window = window;

    QObject::connect(
                window,
                SIGNAL(sig_send_command(int, int)),
                &handler,
                SLOT(send_command(int, int))
    );

    QObject::connect(
                &receiverThread,
                SIGNAL(appendReceivedText(QVariant)),
                window,
                SLOT(onAppendReceivedtext(QVariant))
    );

    QObject::connect(
                COMHandler::com,
                &QSerialPort::readyRead,
                &receiverThread,
                &COMHandler::readFromCOM
    );

    receiverThread.window = window;
   // receiverThread.methodToStartThreadWith = &COMHandler::receiveReport;
   // receiverThread.start();

    app.exec();
    receiverThread.terminate();
    COMHandler::finishCOM();
    return 0;
}
