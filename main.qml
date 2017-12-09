import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQml 2.2

Window {
    visible: true
    width: 800
    height: 600
    color: "#ffffff"
    opacity: 1
    title: qsTr("Kinda Trimble")

    signal sigSendCommandPos()
    signal sigSendCommandIOOptions()
    function onAppendReceivedtext(s) {
        receivedText.text += s+"\n\n"
    }

    Text {
        id: receiverLabel
        x: 475
        y: 17
        width: 138
        height: 24
        text: qsTr("Полученная информация:")
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.italic: true
    }

    Text {
        id: receivedText
        x: 453
        y: 52
        width: 334
        height: 523
        text: qsTr("")
        font.pixelSize: 14
    }

    Button {
        id: sendCommandPosButton
        x: 299
        y: 105
        width: 162
        height: 29
        text: qsTr("Запросить позицию")
        onClicked: {
            sigSendCommandPos();
        }
    }

    Button {
        id: sendCommandIOOptionsButton
        x: 36
        y: 105
        width: 229
        height: 29
        text: qsTr("Настроить ввод-вывод")
        onClicked: {
            sigSendCommandIOOptions();
        }
    }

    CheckBox {
        id: eCEFcheck
        objectName: "eCEFcheck"
        x: 30
        y: 39
        text: qsTr("ECEF (XYZ)")
        checked: true
        onCheckedChanged: {
            // send a signal to code
        }
    }

    CheckBox {
        id: lLAcheck
        objectName: "lLAcheck"
        x: 30
        y: 65
        text: qsTr("LLA")
        checked: true
        onCheckedChanged: {

        }
    }

    RadioButton {
        id: singlePrecRB
        x: 180
        y: 39
        text: qsTr("single")
        onCheckedChanged: {
            doublePrecRB.checked = !(singlePrecRB.checked)
        }
    }

    RadioButton {
        id: doublePrecRB
        objectName: "doublePrecRB"
        x: 180
        y: 65
        text: qsTr("double")
        checked: true
        onCheckedChanged: {
            singlePrecRB.checked = !(doublePrecRB.checked)
        }
    }

    Label {
        id: label
        x: 180
        y: 17
        text: qsTr("Точность данных")
    }

    RadioButton {
        id: gpsRB
        objectName: "gpsTimeRB"
        x: 306
        y: 39
        text: qsTr("GPS")
        onCheckedChanged: {
            utcRB.checked = !(gpsRB.checked)
        }
    }

    RadioButton {
        id: utcRB
        x: 306
        y: 65
        text: qsTr("UTC")
        checked: true
        onCheckedChanged: {
            gpsRB.checked = !(utcRB.checked)
        }
    }

    Label {
        id: label1
        x: 306
        y: 17
        text: qsTr("Временные метки")
    }

    Label {
        id: label2
        x: 30
        y: 17
        text: qsTr("Данные о позиции")
    }

    TabView {
        id: tabView
        x: 19
        y: 164
        width: 416
        height: 411

        Tab {
            id: pos
            title: "Pos"
        }
    }
}
