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

        TextField {
            id: init_x_text
            x: 43
            y: 54
            width: 151
            height: 20
            placeholderText: qsTr("Ось Х")
            objectName: "init_x_text"
        }

        TextField {
            id: init_y_text
            x: 43
            y: 80
            width: 151
            height: 20
            placeholderText: qsTr("Ось Y")
            objectName: "init_y_text"
        }

        TextField {
            id: init_z_text
            x: 43
            y: 106
            width: 151
            height: 20
            placeholderText: qsTr("Ось Z")
            objectName: "init_z_text"
        }

        Label {
            id: label3
            x: 32
            y: 23
            text: qsTr("В Картезианских координатах (XYZ ECEF):")
        }

        Button {
            id: set_init_xyz_pos_button
            x: 233
            y: 64
            width: 98
            height: 52
            text: qsTr("Задать")
        }

        TextField {
            id: init_lat_text
            x: 43
            y: 204
            width: 151
            height: 20
            placeholderText: qsTr("Latitude (широта)")
            objectName: "init_lat_text"
        }

        TextField {
            id: init_long_text
            x: 43
            y: 230
            width: 151
            height: 20
            placeholderText: qsTr("Longitude (долгота)")
            objectName: "init_long_text"
        }

        TextField {
            id: init_alt_text
            x: 43
            y: 256
            width: 151
            height: 20
            placeholderText: qsTr("Altitude (высота)")
            objectName: "init_alt_text"
        }

        Label {
            id: label7
            x: 36
            y: 173
            text: qsTr("В координатах широты-долготы-высоты:")
        }

        Button {
            id: set_init_lla_pos_button
            x: 233
            y: 214
            width: 98
            height: 52
            text: qsTr("Задать")
        }

        Button {
            id: request_pos_vel_button
            x: 43
            y: 395
            width: 295
            height: 59
            text: qsTr("Запросить текущую позицию (а также\nскорость и отметку времени, если вкл.)")
            onClicked: {
                sig_send_COMMAND_REQUEST_STATUS_AND_POS();
            }
        }

    }
}
