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
            id: satellite_selection_info_button
            x: 17
            y: 36
            width: 162
            height: 51
            text: qsTr("Информация о\nвыбранных спутниках")
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_GPS_SATELLITES, 0);
            }
        }

        Button {
            id: satellite_signal_levels_button
            x: 209
            y: 36
            width: 162
            height: 51
            text: qsTr("Уровень сигнала\nвыбранных спутников")
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_SIGNAL_LEVELS, 0);
            }
        }

        Label {
            id: label
            x: 25
            y: 132
            text: qsTr("Режим одного спутника:")
        }

        Button {
            id: auto_one_satellite_selection_button
            x: 25
            y: 168
            width: 154
            height: 46
            text: qsTr("Выбрать наилучший\nавтоматически")
            onClicked: {
                sig_send_command(_COMMAND_SATELLITE_SELECTION, 0);
            }
        }

        Button {
            id: number_one_satellite_selection_button
            x: 25
            y: 239
            width: 154
            height: 46
            text: qsTr("Выбрать по указанному\nномеру (1 - 32)")
            onClicked: {
                sig_send_command(_COMMAND_SATELLITE_SELECTION, satellite_selection_spinner.value.toFixed());
            }
        }

        SpinBox {
            id: satellite_selection_spinner
            x: 204
            y: 252
            width: 60
            height: 20
            maximumValue: 32
            minimumValue: 1
        }

        Rectangle {
            id: rectangle1
            x: 0
            y: 315
            width: 400
            height: 2
            color: "#7d7d7d"
        }

        SpinBox {
            id: satellite_selection_spinner1
            x: 311
            y: 379
            width: 60
            height: 20
            maximumValue: 32
            minimumValue: 0
            objectName: "satellite_selection_spinner1"
        }

        Label {
            id: label1
            x: 25
            y: 339
            text: qsTr("Запрос системной информации о спутнике:")
        }

        Label {
            id: label2
            x: 204
            y: 381
            text: qsTr("Номер (0 - 32):")
        }

        Button {
            id: button
            x: 266
            y: 444
            width: 105
            height: 35
            text: qsTr("Запрос")
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA, 1);
            }
        }

        Label {
            id: label3
            x: 25
            y: 381
            text: qsTr("Тип информации:")
        }

        ComboBox {
            id: comboBox
            x: 25
            y: 410
            width: 174
            height: 22
            model: ["Альманах", "Здоровье, toa, WNa", "Ионосфера", "UTC", "Ephemeris"]
            objectName: "typeOfDataComboBox"
        }
    }
}
