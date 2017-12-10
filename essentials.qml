import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    width: 400
    height: 500
    opacity: 1
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

        Button {
            id: hardware_version_button
            x: 94
            y: 18
            width: 193
            height: 44
            text: qsTr("Версия железа")
        }

        Button {
            id: hardware_components_button
            x: 94
            y: 82
            width: 193
            height: 44
            text: qsTr("Информация о компонентах")
        }

        Button {
            id: cold_reset_button
            x: 108
            y: 223
            width: 165
            height: 44
            text: qsTr("Холодная перезагрузка")
        }

        Button {
            id: warm_reset_button
            x: 108
            y: 287
            width: 165
            height: 44
            text: qsTr("Теплая перезагрузка")
        }

        Button {
            id: hot_reset_button
            x: 108
            y: 351
            width: 165
            height: 44
            text: qsTr("Горячая перезагрузка")
        }

        Button {
            id: factory_reset_button
            x: 267
            y: 426
            width: 111
            height: 57
            text: qsTr("Полный сброс")
            enabled: false
        }

        Button {
            id: software_version_button
            x: 94
            y: 146
            width: 193
            height: 44
            text: qsTr("Версия ПО")
        }

        CheckBox {
            id: confirm_factory_reset_chk
            x: 48
            y: 435
            width: 197
            height: 40
            text: qsTr("Да, я уверен в том, что\nхочу сделать полный сброс")
            onCheckedChanged: {
                factory_reset_button.enabled = confirm_factory_reset_chk.checked
            }
        }
    }
}
