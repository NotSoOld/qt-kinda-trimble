import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    width: 400
    height: 500
    opacity: 1
    anchors.fill: parent
    objectName: "timingPacketItem"

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
            height: 50
            text: qsTr("Прислать основной пакет\nинформации по таймингу немедленно")
            onClicked: {
                timingPacketType = 0;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }

        Button {
            id: button1
            x: 25
            y: 87
            width: 351
            height: 68
            text: qsTr("Прислать основной пакет\nинформации по таймингу\nпосле следующего расчета PPS")
            onClicked: {
                timingPacketType = 1;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }

        Button {
            id: button2
            x: 25
            y: 205
            width: 351
            height: 44
            text: qsTr("Прислать дополнительный пакет\nинформации по таймингу немедленно")
            onClicked: {
                timingPacketType = 0;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_SUPPL_TIMING_PACKET);
            }
        }

        Button {
            id: button3
            x: 25
            y: 263
            width: 351
            height: 63
            text: qsTr("Прислать дополнительный пакет\nинформации по таймингу\nпосле следующего расчета PPS")
            onClicked: {
                timingPacketType = 1;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_SUPPL_TIMING_PACKET);
            }
        }

        Button {
            id: button4
            x: 18
            y: 390
            width: 365
            height: 44
            text: qsTr("Прислать оба пакета после расчета PPS")
            onClicked: {
                timingPacketType = 2;
                sig_send_command(_COMMAND_SUPER, _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET);
            }
        }
    }
}
