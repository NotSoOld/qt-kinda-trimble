import QtQuick 2.1
//import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

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

        Button {
            id: request_io_options_button
            x: 35
            y: 8
            width: 330
            height: 44
            text: qsTr("Запросить текущие настройки ввода-вывода")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SET_IO_OPTIONS, -1);
            }
        }

        Rectangle {
            id: rectangle1
            x: 0
            y: 64
            width: 400
            height: 2
            color: "#7d7d7d"
        }

        Button {
            id: save_to_flash_io_options_button
            x: 31
            y: 443
            width: 323
            height: 42
            text: qsTr("Сохранить настройки (из ОЗУ устройства) в ПЗУ")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SUPER, _CMDSUB_WRITE_CONFIG_TO_FLASH);
            }
        }

        CheckBox {
            id: eCEFcheck
            objectName: "eCEFcheck"
            x: 19
            y: 103
            text: qsTr("ECEF (XYZ)")
            checked: true
        }

        CheckBox {
            id: lLAcheck
            objectName: "lLAcheck"
            x: 19
            y: 135
            text: qsTr("LLA")
            checked: true
        }

        ButtonGroup {
            buttons: prec_column.children
        }

        Column {
            id: prec_column
            x: 10
            y: 81
            spacing: -10

            Label {
                id: label
                x: 149
                y: 0
                text: qsTr("Точность:")
            }
            Rectangle {
                width: 7
                height: 20
                color: "#00ffffff"
            }

            RadioButton {
                id: singlePrecRB
                x: 149
                y: 103
                text: qsTr("single")
                autoExclusive: true
                checked: true
            }

            RadioButton {
                id: doublePrecRB
                objectName: "doublePrecRB"
                x: 149
                y: 146
                text: qsTr("double")
                checked: true
            }

        }

        ButtonGroup {
            buttons: time_column.children
        }

        Column {
            id: time_column
            x: -3
            y: 81
            spacing: -10

            Label {
                id: label1
                x: 273
                y: 81
                text: qsTr("Временные метки:")
            }

            Rectangle {
                width: 7
                height: 20
                color: "#00ffffff"
            }
            RadioButton {
                id: gpsRB
                objectName: "gpsTimeRB"
                x: 273
                y: 103
                text: qsTr("GPS")
                checkable: true
                checked: true
            }

            RadioButton {
                id: utcRB
                x: 273
                y: 146
                text: qsTr("UTC")
                checked: false
            }
        }

        Label {
            id: label2
            x: 19
            y: 81
            text: "Данные о позиции:"
        }

        ButtonGroup {
            buttons: hae_msl_column.children
        }

        Column {
            id: hae_msl_column
            x: -1
            y: 181
            spacing: -10


            RadioButton {
                id: haeRB
                x: 22
                y: 181
                text: qsTr("HAE")
                checked: true
                objectName: "haeRB"
            }

            RadioButton {
                id: mslRB
                x: 22
                y: 223
                text: qsTr("Геоид MSL")
                checked: false
                objectName: "mslRB"
            }
        }

        CheckBox {
            id: ecef_vel_check
            x: 238
            y: 214
            text: qsTr("ECEF")
            checked: true
            objectName: "ecef_vel_check"
        }

        CheckBox {
            id: enu_vel_check
            x: 149
            y: 214
            text: qsTr("ENU")
            checked: true
            objectName: "enu_vel_check"
        }

        Label {
            id: label3
            x: 181
            y: 198
            width: 118
            height: 21
            text: "Данные о скорости:"
        }

        CheckBox {
            id: raw_data_report_check
            x: 19
            y: 274
            text: qsTr("Прием пакетов с сырыми данными об измерениях")
            objectName: "raw_data_report_check"
        }

        ButtonGroup {
            buttons: out_column.children
        }

        Column {
            id: out_column
            x: -1
            y: 310
            spacing: -10

            RadioButton {
                id: aem_out_RB
                x: 19
                y: 313
                width: 140
                height: 40
                text: qsTr("Вывод в а.е.м.")
                objectName: "aem_out_RB"
            }

            RadioButton {
                id: dbhz_out_RB
                x: 19
                y: 350
                text: qsTr("Вывод в дБ-Гц")
                checked: true
                objectName: "dbhz_out_RB"
            }
        }

        Button {
            id: send_io_options_button
            x: 31
            y: 395
            width: 323
            height: 42
            text: "Отправить настройки ввода-вывода в ОЗУ"
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SET_IO_OPTIONS, 0);
            }
        }
    }
}
