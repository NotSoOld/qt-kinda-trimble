import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

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
            onClicked: {
                sig_send_command(_COMMAND_FIRMWARE_INFO, _CMDSUB_FIRMWARE_VERSION);
            }
        }

        Button {
            id: hardware_components_button
            x: 77
            y: 83
            width: 246
            height: 44
            text: qsTr("Информация о компонентах")
            onClicked: {
                sig_send_command(_COMMAND_FIRMWARE_INFO, _CMDSUB_HARDWARE_COMPONENT_INFO);
            }
        }

        Button {
            id: cold_reset_button
            x: 94
            y: 223
            width: 193
            height: 44
            text: qsTr("Холодная перезагрузка")
            onClicked: {
                sig_send_command(_COMMAND_INITIATE_RESET, 0x4B);
            }
        }

        Button {
            id: warm_reset_button
            x: 94
            y: 287
            width: 193
            height: 44
            text: qsTr("Теплая перезагрузка")
            onClicked: {
                sig_send_command(_COMMAND_INITIATE_RESET, 0x0E);
            }
        }

        Button {
            id: hot_reset_button
            x: 94
            y: 351
            width: 193
            height: 44
            text: qsTr("Горячая перезагрузка")
            onClicked: {
                sig_send_command(_COMMAND_INITIATE_HOT_RESET, 0);
            }
        }

        Button {
            id: factory_reset_button
            x: 251
            y: 426
            width: 141
            height: 57
            text: qsTr("Полный сброс")
            enabled: false
            onClicked: {
                sig_send_command(_COMMAND_INITIATE_RESET, 0x46);
            }
        }

        Button {
            id: software_version_button
            x: 94
            y: 146
            width: 193
            height: 44
            text: qsTr("Версия ПО")
            font.pointSize: 8
            onClicked: {
                sig_send_command(_COMMAND_REQEST_SOFTWARE_VERSION, 0);
            }
        }

        CheckBox {
            id: confirm_factory_reset_chk
            x: 41
            y: 421
            width: 237
            height: 68
            text: qsTr("Да, я уверен в том, что\nхочу сделать полный сброс")
            padding: 0
            leftPadding: 0
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            font.pointSize: 8
            onCheckedChanged: {
                factory_reset_button.enabled = confirm_factory_reset_chk.checked
            }
        }
    }
}
