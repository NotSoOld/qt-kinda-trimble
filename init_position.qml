import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

// Вкладка с текстовыми полями для задания начальной позиции внутри GPS-модуля.
// Также есть кнопка для запроса текущей позиции досрочно, НО GPS-модуль упорно
// не хочет ее присылать. Видимо, это работает как-то иначе, например, он пришлет после того,
// как фиксы сделает, или типа того...

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
            x: 36
            y: 54
            width: 174
            height: 30
            leftPadding: 0
            padding: 0
            bottomPadding: 0
            topPadding: 0
            placeholderText: qsTr("Ось Х (м)")
            objectName: "init_x_text"
        }

        TextField {
            id: init_y_text
            x: 36
            y: 90
            width: 174
            height: 30
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            leftPadding: 0
            padding: 0
            placeholderText: qsTr("Ось Y (м)")
            objectName: "init_y_text"
        }

        TextField {
            id: init_z_text
            x: 36
            y: 124
            width: 174
            height: 30
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            leftPadding: 0
            padding: 0
            placeholderText: qsTr("Ось Z (м)")
            objectName: "init_z_text"
        }

        Label {
            id: label3
            x: 36
            y: 26
            text: qsTr("В Картезианских координатах (XYZ ECEF):")
        }

        Button {
            id: set_init_xyz_pos_button
            x: 261
            y: 79
            width: 98
            height: 52
            text: qsTr("Задать")
            onClicked: {
                sig_send_command(_COMMAND_ACCURATE_INIT_POS_XYZ, 0);
            }
        }

        TextField {
            id: init_lat_text
            x: 36
            y: 230
            width: 195
            height: 30
            bottomPadding: 0
            rightPadding: 0
            topPadding: 0
            leftPadding: 0
            padding: 0
            placeholderText: qsTr("Latitude (широта, рад)")
            objectName: "init_lat_text"
        }

        TextField {
            id: init_long_text
            x: 36
            y: 266
            width: 195
            height: 30
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            leftPadding: 0
            padding: 0
            placeholderText: qsTr("Longitude (долгота, рад)")
            objectName: "init_long_text"
        }

        TextField {
            id: init_alt_text
            x: 36
            y: 302
            width: 195
            height: 30
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            leftPadding: 0
            padding: 0
            placeholderText: qsTr("Altitude (высота, м)")
            objectName: "init_alt_text"
        }

        Label {
            id: label7
            x: 36
            y: 202
            text: qsTr("В координатах широты-долготы-высоты:")
        }

        Button {
            id: set_init_lla_pos_button
            x: 261
            y: 255
            width: 98
            height: 52
            text: qsTr("Задать")
            onClicked: {
                sig_send_command(_COMMAND_ACCURATE_INIT_POS_LLA, 0);
            }
        }

        Button {
            id: request_pos_vel_button
            x: 43
            y: 382
            width: 295
            height: 72
            text: qsTr("Запросить текущую позицию (а также\nскорость и отметку времени, если вкл.)")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_STATUS_AND_POS, 0);
            }
        }

    }
}
