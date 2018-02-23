import QtQuick 2.1
import QtQuick.Controls 1.0
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
            id: satellite_selection_info_button
            x: 17
            y: 36
            width: 176
            height: 59
            text: qsTr("Информация о\nвыбранных спутниках")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_GPS_SATELLITES, 0);
            }
        }

        Button {
            id: satellite_signal_levels_button
            x: 209
            y: 36
            width: 174
            height: 59
            text: qsTr("Уровень сигнала\nвыбранных спутников")
            font.pointSize: 7
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
            width: 186
            height: 58
            text: qsTr("Выбрать наилучший\nавтоматически")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SATELLITE_SELECTION, 0);
            }
        }

        Button {
            id: number_one_satellite_selection_button
            x: 25
            y: 239
            width: 186
            height: 56
            text: qsTr("Выбрать по указанному\nномеру (1 - 32)")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SATELLITE_SELECTION, satellite_selection_spinner.value.toFixed());
            }
        }

        SpinBox {
            id: satellite_selection_spinner
            x: 245
            y: 252
            width: 118
            height: 33

            to: 32
            from: 1
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
            x: 254
            y: 397
            width: 129
            height: 28
            to: 32
            from: 0
            objectName: "satellite_selection_spinner1"
        }

        Label {
            id: label1
            x: 25
            y: 331
            width: 256
            height: 24
            text: qsTr("Запрос системной информации о спутнике:")
        }

        Label {
            id: label2
            x: 266
            y: 366
            text: qsTr("Номер (0 - 32):")
        }

        Button {
            id: button
            x: 238
            y: 439
            width: 133
            height: 40
            text: qsTr("Запрос")
            onClicked: {
                // Подкод "1" - см. документацию.
                sig_send_command(_COMMAND_REQUEST_SATELLITE_SYSTEM_DATA, 1);
            }
        }

        Label {
            id: label3
            x: 25
            y: 375
            text: qsTr("Тип информации:")
        }

        ComboBox {
            id: comboBox
            x: 25
            y: 397
            width: 174
            height: 39
            model: ["Альманах", "Здоровье, toa, WNa", "Ионосфера", "UTC", "Ephemeris"]
            objectName: "typeOfDataComboBox"
        }
    }
}
