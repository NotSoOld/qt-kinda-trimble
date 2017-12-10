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
            id: button
            x: 31
            y: 14
            width: 323
            height: 35
            text: qsTr("Запросить текущие настройки ввода-вывода")
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
            id: button1
            x: 31
            y: 435
            width: 323
            height: 35
            text: qsTr("Сохранить настройки (из ОЗУ устройства) в ПЗУ")
        }

        CheckBox {
            id: eCEFcheck
            objectName: "eCEFcheck"
            x: 19
            y: 103
            text: qsTr("ECEF (XYZ)")
            checked: true
            onCheckedChanged: {
                // send a signal to code
            }
        }

        CheckBox {
            id: lLAcheck
            objectName: "lLAcheck"
            x: 19
            y: 129
            text: qsTr("LLA")
            checked: true
            onCheckedChanged: {

            }
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
            id: singlePrecRB1
            x: 19
            y: 155
            text: qsTr("HAE")
        }

        RadioButton {
            id: doublePrecRB1
            x: 19
            y: 181
            text: qsTr("Геоид MSL")
            checked: true
            objectName: "doublePrecRB"
        }

        CheckBox {
            id: eCEFcheck1
            x: 149
            y: 188
            text: qsTr("ECEF")
            checked: true
            objectName: "eCEFcheck"
        }

        CheckBox {
            id: lLAcheck1
            x: 149
            y: 214
            text: qsTr("ENU")
            checked: true
            objectName: "lLAcheck"
        }

        Label {
            id: label3
            x: 149
            y: 166
            text: "Данные о скорости:"
        }

        CheckBox {
            id: checkBox
            x: 19
            y: 251
            text: qsTr("Прием пакетов с сырыми данными об измерениях")
        }

        RadioButton {
            id: singlePrecRB2
            x: 19
            y: 277
            text: qsTr("Вывод в а.е.м.")
        }

        RadioButton {
            id: doublePrecRB2
            x: 19
            y: 303
            text: qsTr("Вывод в дБ-Гц")
            checked: true
            objectName: "doublePrecRB"
        }

        Button {
            id: button2
            x: 31
            y: 350
            width: 323
            height: 35
            text: "Отправить настройки ввода-вывода в ОЗУ"
        }


    }
}
