#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickStyle>
#include "comhandler.h"
#include "qmldatahelper.h"

// Отсюда начинается выполнение программы.

COMHandler handler;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Стиль приложения касательно библиотек Qt Quick 2.
    QQuickStyle::setStyle("Material");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Получение ссылки на главное окно приложения, которое постоянно используется при доступе к элементам интерфейса QML.
    QMLDataHelper::mainWindow = qobject_cast<QQuickWindow *>(engine.rootObjects().value(0));
    if (!QMLDataHelper::mainWindow) {
        qDebug() << QString("Не удается найти основное окно");
        exit(1);
    }

    // Связка для получения сигналов на отправку пакетов запросов.
    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_send_command(int, int)),
                &handler,
                SLOT(send_command(int, int))
    );

    // Связка для отправки сообщений в лог в интерфейсе.
    QObject::connect(
                &handler,
                SIGNAL(appendReceivedText(QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onAppendReceivedtext(QVariant))
    );

    // Связка для получения сигнала-команды на открытие и конфигурирование VirtualCOM.
    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_open_port(int, int, int, int, int, int)),
                &handler,
                SLOT(configureCOM(int, int, int, int, int, int))
    );

    // Связка для получения сигнала-просьбы обновить список портов в окне инициализации VirtualCOM.
    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_get_serial_ports()),
                &handler,
                SLOT(getSerialPortsList())
    );

    // При первом запуске проще запустить этот слот отсюда, чем из QML.
    handler.getSerialPortsList();

    // Запуск, собственно, приложения.
    app.exec();
    // Перед завершением работы закроем порт.
    handler.finishCOM();
    return 0;
}