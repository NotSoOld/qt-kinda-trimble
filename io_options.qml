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

        Button {
            id: request_io_options_button
            x: 31
            y: 14
            width: 323
            height: 35
            text: qsTr("Запросить текущие настройки ввода-вывода")
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
            y: 435
            width: 323
            height: 35
            text: qsTr("Сохранить настройки (из ОЗУ устройства) в ПЗУ")
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
            y: 129
            text: qsTr("LLA")
            checked: true
        }

        RadioButton {
            id: singlePrecRB
            x: 149
            y: 103
            text: qsTr("single")
            onCheckedChanged: {
                doublePrecRB.checked = !(singlePrecRB.checked)
            }
        }

        RadioButton {
            id: doublePrecRB
            objectName: "doublePrecRB"
            x: 149
            y: 129
            text: qsTr("double")
            checked: true
            onCheckedChanged: {
                singlePrecRB.checked = !(doublePrecRB.checked)
            }
        }

        Label {
            id: label
            x: 149
            y: 81
            text: qsTr("Точность данных:")
        }

        RadioButton {
            id: gpsRB
            objectName: "gpsTimeRB"
            x: 273
            y: 103
            text: qsTr("GPS")
            onCheckedChanged: {
                utcRB.checked = !(gpsRB.checked)
            }
        }

        RadioButton {
            id: utcRB
            x: 273
            y: 129
            text: qsTr("UTC")
            checked: true
            onCheckedChanged: {
                gpsRB.checked = !(utcRB.checked)
            }
        }

        Label {
            id: label1
            x: 273
            y: 81
            text: qsTr("Временные метки:")
        }

        Label {
            id: label2
            x: 19
            y: 81
            text: "Данные о позиции:"
        }

        RadioButton {
            id: haeRB
            x: 19
            y: 155
            text: qsTr("HAE")
            objectName: "haeRB"
            onCheckedChanged: {
                mslRB.checked = !(haeRB.checked)
            }
        }

        RadioButton {
            id: mslRB
            x: 19
            y: 181
            text: qsTr("Геоид MSL")
            checked: true
            objectName: "mslRB"
            onCheckedChanged: {
                haeRB.checked = !(mslRB.checked)
            }
        }

        CheckBox {
            id: ecef_vel_check
            x: 149
            y: 188
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
            x: 149
            y: 166
            text: "Данные о скорости:"
        }

        CheckBox {
            id: raw_data_report_check
            x: 19
            y: 251
            text: qsTr("Прием пакетов с сырыми данными об измерениях")
            objectName: "raw_data_report_check"
        }

        RadioButton {
            id: aem_out_RB
            x: 19
            y: 277
            text: qsTr("Вывод в а.е.м.")
            objectName: "aem_out_RB"
            onCheckedChanged: {
                dbhz_out_RB.checked = !(aem_out_RB.checked)
            }
        }

        RadioButton {
            id: dbhz_out_RB
            x: 19
            y: 303
            text: qsTr("Вывод в дБ-Гц")
            checked: true
            objectName: "dbhz_out_RB"
            onCheckedChanged: {
                aem_out_RB.checked = !(dbhz_out_RB.checked)
            }
        }

        Button {
            id: send_io_options_button
            x: 31
            y: 350
            width: 323
            height: 35
            text: "Отправить настройки ввода-вывода в ОЗУ"
            onClicked: {
                sig_send_command(_COMMAND_SET_IO_OPTIONS, 0);
            }
        }
    }
}
