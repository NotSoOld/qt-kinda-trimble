#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickStyle>
#include "comhandler.h"
#include "qmldatahelper.h"

// Отсюда начинается выполнение программы.

// Когда-то COMHandler был многопоточным... Теперь существование сразу двух его экземпляров
// вызывает вопросы о целесообразности.
COMHandler handler;
COMHandler receiverThread;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Стиль приложения.
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
                &receiverThread,
                SIGNAL(appendReceivedText(QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onAppendReceivedtext(QVariant))
    );

    // Связка для получения сигнала-команды на открытие и конфигурирование VirtualCOM.
    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_open_port(int, int, int, int, int, int)),
                &receiverThread,
                SLOT(configureCOM(int, int, int, int, int, int))
    );

    // Связка для получения сигнала-просьбы обновить список портов в окне инициализации VirtualCOM.
    QObject::connect(
                QMLDataHelper::mainWindow,
                SIGNAL(sig_get_serial_ports()),
                &receiverThread,
                SLOT(getSerialPortsList())
    );

    // Связка для обновления графиков в интерфейсе (пока что не работает).
    QObject::connect(
                &receiverThread,
                SIGNAL(newValuesGained(QVariant, QVariant)),
                QMLDataHelper::mainWindow,
                SLOT(onGainNewValues(QVariant, QVariant))
    );

    // При первом запуске проще запустить этот слот отсюда, чем из QML.
    receiverThread.getSerialPortsList();

    // Тоже от многопоточности осталось
   // receiverThread.methodToStartThreadWith = &COMHandler::receiveReport;
   // receiverThread.start();

    // Запуск, собственно, приложения.
    app.exec();
    //receiverThread.terminate();
    // Перед завершением работы закроем порт.
    handler.finishCOM();
    return 0;
}
