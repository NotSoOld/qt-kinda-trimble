import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

// Вкладка настройки авторассылки пакетов.

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
            y: 82
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
            y: 117
            text: qsTr("Остальные пакеты (позиция, скорость,\nсырые измерения, список спутников)")
            checked: true
            objectName: "otherPacketsMaskingBit"
        }

        Button {
            id: send_autopacket_masking_options_button
            x: 223
            y: 177
            width: 149
            height: 86
            text: "Отправить\nнастройки\nмаскирования"
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SUPER, _CMDSUB_SET_PACKET_BROADCAST_MASK);
            }
        }

        Label {
            id: label3
            x: 21
            y: 177
            width: 171
            height: 113
            // Почему - см. документацию про broadcast packets.
            text: "Примечание. Перечисленные у третьего пункта пакеты могут маскироваться другими пакетами и не приходить, даже если флажок установлен."
            font.bold: false
            font.italic: true
            wrapMode: Text.WordWrap
        }
    }
}
