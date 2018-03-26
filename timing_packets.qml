import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

// Вкладка для получения пакетов по таймингу, если они не приходят сами.

Item {
    width: 400
    height: 500
    opacity: 1
    anchors.fill: parent
    objectName: "timingPacketItem"

    // Сигнал отправки пакета и так уже передает код и подкод суперпакета, а нужен еще третий байт для различения типа пакета.
    // Он хранится в этой переменной и забирается кодом С++ во время формирования пакета.
    property int timingPacketType: 0;

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 400
        height: 500
        color: "#ffffff"
        radius: 2
        border.color: "#cc898989"
        border.width: 1

        Button {
            id: button
            x: 25
            y: 24
            width: 351
            height: 68
            text: qsTr("Прислать основной пакет\nинформации по таймингу немедленно")
            font.pointSize: 7
            onClicked: {
                timingPacketType = 0;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }

        Button {
            id: button1
            x: 25
            y: 105
            width: 351
            height: 76
            text: qsTr("Прислать основной пакет\nинформации по таймингу\nпосле следующего расчета PPS")
            font.pointSize: 7
            onClicked: {
                timingPacketType = 1;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }

        Button {
            id: button2
            x: 25
            y: 231
            width: 351
            height: 60
            text: qsTr("Прислать дополнительный пакет\nинформации по таймингу немедленно")
            font.pointSize: 7
            onClicked: {
                timingPacketType = 0;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_SUPPL_TIMING_PACKET);
            }
        }

        Button {
            id: button3
            x: 25
            y: 308
            width: 351
            height: 76
            text: qsTr("Прислать дополнительный пакет\nинформации по таймингу\nпосле следующего расчета PPS")
            font.pointSize: 7
            onClicked: {
                timingPacketType = 1;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_SUPPL_TIMING_PACKET);
            }
        }

        Button {
            id: button4
            x: 18
            y: 422
            width: 365
            height: 44
            text: qsTr("Прислать оба пакета после расчета PPS")
            font.pointSize: 7
            onClicked: {
                timingPacketType = 2;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }
    }
}
