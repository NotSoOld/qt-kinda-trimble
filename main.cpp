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
                SIGNAL(sig_open_port(int, int, int, int, int, int)),
                &receiverThread,
                SLOT(configureCOM(int, int, int, int, int, int))
    );

    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_get_serial_ports()),
                &receiverThread,
                SLOT(getSerialPortsList())
    );

    QObject::connect(
                &receiverThread,
                SIGNAL(newValuesGained(QVariant, QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onGainNewValues(QVariant, QVariant))
    );

    // При первом запуске проще запустить этот слот отсюда, чем из QML.
    receiverThread.getSerialPortsList();

   // receiverThread.methodToStartThreadWith = &COMHandler::receiveReport;
   // receiverThread.start();
/*
    QVariantList list;
    list.append("foo");
    list.append("bar");
    list.append("baz");
    QObject *comboBox = QMLDataHelper::mainWindow->findChild<QObject *>("com_init_window");
    comboBox = comboBox->findChild<QObject *>("baudComboBox");
    comboBox->setProperty("model", list);
    comboBox->setProperty("currentIndex", 1);
*/
    app.exec();
    receiverThread.terminate();
    COMHandler::finishCOM();
    return 0;
}
