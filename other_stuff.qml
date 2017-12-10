import QtQuick 2.1
import QtQuick.Controls 1.0

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
            y: 144
            width: 175
            height: 44
            text: qsTr("Последнее измерение\n(в сыром виде со спутника)")
        }

        Button {
            id: button1
            x: 209
            y: 144
            width: 175
            height: 44
            text: qsTr("Статус трекинга спутника")
        }

        SpinBox {
            id: spinBox
            x: 169
            y: 99
            width: 66
            height: 20
            maximumValue: 32
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
            y: 202
            width: 175
            height: 28
            text: qsTr("Включить спутник")
        }

        Button {
            id: button3
            x: 209
            y: 202
            width: 175
            height: 28
            text: qsTr("Выключить спутник")
        }

        Button {
            id: button4
            x: 16
            y: 310
            width: 175
            height: 47
            text: qsTr("Принимать здоровье\nспутника во внимание")
        }

        Button {
            id: button5
            x: 209
            y: 310
            width: 175
            height: 47
            text: qsTr("Игнорировать здоровье\nспутника")
        }

        Button {
            id: button6
            x: 61
            y: 247
            width: 282
            height: 33
            text: qsTr("Запросить статус включения спутников")
        }

        Button {
            id: button7
            x: 45
            y: 376
            width: 310
            height: 33
            text: qsTr("Запросить статус значения здоровья спутников")
        }


    }
}
