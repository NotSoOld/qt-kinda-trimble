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

        Label {
            id: label
            x: 28
            y: 54
            text: qsTr("Ось Х:")
        }

        TextField {
            id: init_x_text
            x: 87
            y: 54
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label1
            x: 28
            y: 80
            text: qsTr("Ось Y:")
        }

        TextField {
            id: init_y_text
            x: 87
            y: 80
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label2
            x: 28
            y: 106
            text: qsTr("Ось Z:")
        }

        TextField {
            id: init_z_text
            x: 87
            y: 106
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label3
            x: 32
            y: 23
            text: qsTr("В Картезианских координатах (XYZ ECEF):")
        }

        Button {
            id: button
            x: 262
            y: 64
            width: 98
            height: 52
            text: qsTr("Задать")
        }

        Label {
            id: label4
            x: 32
            y: 204
            text: qsTr("Lat. (широта):")
        }

        TextField {
            id: init_x_text1
            x: 143
            y: 204
            width: 94
            height: 20
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label5
            x: 32
            y: 230
            text: qsTr("Long. (долгота):")
        }

        TextField {
            id: init_y_text1
            x: 143
            y: 230
            width: 94
            height: 20
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label6
            x: 32
            y: 256
            text: qsTr("Alt. (высота):")
        }

        TextField {
            id: init_z_text1
            x: 143
            y: 256
            width: 94
            height: 20
            placeholderText: qsTr("Text Field")
        }

        Label {
            id: label7
            x: 36
            y: 173
            text: qsTr("В координатах широты-долготы-высоты:")
        }

        Button {
            id: button1
            x: 266
            y: 214
            width: 98
            height: 52
            text: qsTr("Задать")
        }

        Button {
            id: button2
            x: 43
            y: 395
            width: 295
            height: 59
            text: qsTr("Запросить текущую позицию (а также\nскорость и отметку времени, если вкл.)")
        }

    }
}
