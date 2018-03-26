import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2

// Еще одна вкладка с кнопками. В основном, позволяет получить какую-либо информацию о спутниках.

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
            id: button
            x: 16
            y: 152
            width: 175
            height: 59
            text: qsTr("Последнее измерение\n(в сыром виде со спутника)")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_LAST_RAW_MEASUREMENT, satellite_selection_spinner.value.toFixed());
            }
        }

        Button {
            id: button1
            x: 209
            y: 152
            width: 175
            height: 59
            text: qsTr("Статус трекинга\nспутника")
            onClicked: {
                sig_send_command(_COMMAND_REQUEST_SATELLITE_TRACKING_STATUS, satellite_selection_spinner.value.toFixed());
            }
        }

        SpinBox {
            id: satellite_selection_spinner
            x: 132
            y: 104
            width: 136
            height: 29
            font.pointSize: 9
            from: 0
            to: 32
            objectName: "satellites_and_health_spinner"
        }

        Label {
            id: label
            x: 43
            y: 17
            text: qsTr("Выберите номер спутника (0 - 32). Все запросы ниже\nбудут совершаться для выбранного спутника.")
        }

        Label {
            id: label1
            x: 30
            y: 55
            text: qsTr("(если выбран 0, информация/запрос будет касаться всех\nспутников, которые задействованы в данный момент)")
        }

        Button {
            id: button2
            x: 16
            y: 224
            width: 175
            height: 42
            text: qsTr("Включить спутник")
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 1);
            }
        }

        Button {
            id: button3
            x: 209
            y: 224
            width: 175
            height: 42
            text: qsTr("Выключить спутник")
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 2);
            }
        }

        Button {
            id: button4
            x: 16
            y: 336
            width: 175
            height: 59
            text: qsTr("Принимать здоровье\nспутника во внимание")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 4);
            }
        }

        Button {
            id: button5
            x: 209
            y: 336
            width: 175
            height: 59
            text: qsTr("Игнорировать здоровье\nспутника")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 5);
            }
        }

        Button {
            id: button6
            x: 16
            y: 277
            width: 368
            height: 44
            text: qsTr("Запросить статус включения всех 32 спутников")
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 3);
            }
        }

        Button {
            id: button7
            x: 16
            y: 413
            width: 368
            height: 56
            text: qsTr("Запросить статус значения здоровья\nвсех 32 спутников")
            font.pointSize: 7
            onClicked: {
                sig_send_command(_COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH, 6);
            }
        }


    }
}
