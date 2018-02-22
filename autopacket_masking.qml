import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    width: 400
    height: 500
    anchors.fill: parent

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

        CheckBox {
            id: primaryPacketMaskingBit
            objectName: "primaryPacketMaskingBit"
            x: 21
            y: 45
            text: qsTr("Основная информация по таймингу")
            checked: true
        }

        CheckBox {
            id: supplPacketMaskingBit
            objectName: "supplPacketMaskingBit"
            x: 21
            y: 71
            text: qsTr("Дополн. информация по таймингу")
            checked: true
        }

        Label {
            id: label2
            x: 21
            y: 23
            text: "Выберите, какие пакеты нужно регулярно получать:"
            font.bold: false
        }

        CheckBox {
            id: otherPacketsMaskingBit
            x: 21
            y: 97
            text: qsTr("Остальные пакеты (позиция, скорость,\nсырые измерения, список спутников)")
            checked: true
            objectName: "otherPacketsMaskingBit"
        }

        Button {
            id: send_autopacket_masking_options_button
            x: 224
            y: 159
            width: 149
            height: 66
            text: "Отправить\nнастройки\nмаскирования"
            onClicked: {
                sig_send_command(_COMMAND_SUPER, _CMDSUB_SET_PACKET_BROADCAST_MASK);
            }
        }

        Label {
            id: label3
            x: 21
            y: 151
            width: 171
            height: 113
            text: "Примечание. Перечисленные у третьего пункта пакеты могут маскироваться другими пакетами и не приходить, даже если флажок установлен."
            font.bold: false
            font.italic: true
            wrapMode: Text.WordWrap
        }
    }
}
