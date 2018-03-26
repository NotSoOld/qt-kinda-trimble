#ifndef COMHANDLER_H
#define COMHANDLER_H

#include "kinda_trimble_shared.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include "converter.h"
#include "qbytehelper.h"
#include "qmldatahelper.h"
#include <QQuickView>
#include <QQuickItem>
#include "cmdbuilder.h"

// Класс, отвечающий за основы взаимодействия с GPS-модулем через VirtualCOM.
// См. комментарии ниже для подробностей.
class COMHandler : public QObject
{
    Q_OBJECT
public:
    // Метод закрытия порта (заодно удаляет объект порта).
    void finishCOM();

    // Метод, вызывающийся после того, как был получен полный пакет (обнаружена DLE-ETX, см. документацию про строение пакета).
    void receiveReport();

    // Статические члены класса: сам объект порта VirtualCOM, массив текущей прочитанной информации из порта
    // (в момент, когда обнаруживается DLE-ETX, содержит полные данные пакета)...
    static QSerialPort *com;
    static QByteArray readedData;
    // ...последний прочитанный байт, нужен для обнаружения DLE-ETX и байтов экранирования DLE...
    static quint8 previouslyReadedChar;
    // ...и полный список информациЙ о портах в системе, нужен сначала для отображения списка портов в интерфейсе
    // конфигурирования и открытия VirtualCOM, а затем для его непосредственного открытия, т.к. только в этом списке
    // точно хранится нормальное имя порта, через которое его можно открыть.
    static QList<QSerialPortInfo> portsList;

private:
    // Метод запроса практически всей информации, которая отображается в интерфейсе. Отправляет сразу несколько
    // запросов к GPS-модулю, чтобы тот вернул вышеупомянутую информацию для заполнения интерфейса, например,
    // после включения программы, перезагрузки порта или GPS.
    void requestEssentialInfo(unsigned long = 0);

signals:
    // Высылается слоту в QML каждый раз, когда нужно напечатать что-то в лог.
    void appendReceivedText(QVariant s);

    // Для графиков (если руки дойдут)
    void newValuesGained(QVariant nx, QVariant ny);

public slots:
    // Приходит из QML каждый раз, когда пользователь жмет кнопку отправки пакета. Присылает код пакета и подкод или
    // просто первый байт данных (иногда так удобнее).
    void send_command(int, int);

    // Слот, подписанный на приход байта в порт VirtualCOM ("есть, что читать", сигнал readyRead класса QSerialPort).
    void readFromCOM();

    // Вызывается сигналом из QML, когда в окне конфигурирования VirtualCOM нажимается кнопка "Подключиться".
    // Передает параметры открываемого порта, в том числе индекс этого порта в списке portsList (см. выше).
    void configureCOM(int, int, int, int, int, int);

    // Вызывается сигналом из QML (да и вообще вызывается) каждый раз, когда окно конфигурирования VirtualCOM
    // открывается, чтобы наполнить выпадающий список доступными для открытия портами.
    void getSerialPortsList();

};

// Он здесь, потому что был конфликт порядка объявления классов.
#include "packetparser.h"

#endif // COMHANDLER_H
