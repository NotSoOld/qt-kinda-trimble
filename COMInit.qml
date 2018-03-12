import QtQuick 2.1
import QtQuick.Controls 1.5
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

Window {
    id: comInitWindow

    visible: true
    width: 600
    height: 480
    title: "Настройка подключения"

    // Индекс записи о порте передается потому, что сама запись не несет в себе полезной информации об имени порта.
    // Имя порта будет позже выбрано в С++ по этому индексу.
    signal closeWindow(int portIndex, int baud, int dataBits, int parity, int flowControl, int stopBits)

    Text {
        id: text1
        x: 82
        y: 8
        width: 437
        height: 44
        text: qsTr("Настройка подключения по VirtualCOM")
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
    }

    Text {
        id: text2
        x: 18
        y: 99
        text: qsTr("Имя порта:")
        font.pixelSize: 16
    }
/*
    TextField {
        id: portNameTextEdit
        x: 211
        y: 67
        width: 90
        height: 35
        padding: 0
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
        topPadding: 0
        placeholderText: qsTr("COM")
        font.pixelSize: 16
    }
*/
    Text {
        id: text3
        x: 18
        y: 144
        text: qsTr("Скорость (бод):")
        font.pixelSize: 16
    }

    ComboBox {
        id: portsComboBox
        objectName: "portsComboBox"
        x: 115
        y: 88
        width: 465
        height: 40
    }

    ComboBox {
        id: baudComboBox
        objectName: "baudComboBox"
        x: 169
        y: 134
        width: 90
        height: 40
        currentIndex: 3
        model: [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
    }

    Text {
        id: text4
        x: 34
        y: 192
        text: qsTr("Биты данных:")
        font.pixelSize: 16
    }

    ComboBox {
        id: dataBitsComboBox
        x: 169
        y: 182
        width: 90
        height: 40
        currentIndex: 3
        model: [5, 6, 7, 8]
    }

    Text {
        id: text5
        x: 357
        y: 145
        text: qsTr("Четность:")
        font.pixelSize: 16
    }

    ComboBox {
        id: parityComboBox
        x: 462
        y: 135
        width: 90
        height: 40
        currentIndex: 0
        model: ["no", "even", "odd", "space", "mark"]
    }

    Text {
        id: text6
        x: 297
        y: 192
        text: qsTr("Контроль потока:")
        font.pixelSize: 16
    }

    ComboBox {
        id: flowControlComboBox
        x: 462
        y: 182
        width: 90
        height: 40
        currentIndex: 0
        model: ["нет", "аппаратный", "программный"]
    }

    Text {
        id: text7
        x: 52
        y: 243
        text: qsTr("Стоп-биты:")
        font.pixelSize: 16
    }

    ComboBox {
        id: stopBitsComboBox
        x: 168
        y: 233
        width: 90
        height: 40
        currentIndex: 0
        model: ["1", "1.5", "2"]
    }

    Button {
        id: button
        x: 224
        y: 306
        width: 153
        height: 51
        text: qsTr("Подключиться")
        onClicked: {
            comInitWindow.closeWindow(portsComboBox.currentIndex, parseInt(baudComboBox.currentText),
                                      dataBitsComboBox.currentIndex, parityComboBox.currentIndex,
                                      flowControlComboBox.currentIndex, stopBitsComboBox.currentIndex);
        }
    }

    Text {
        id: text8
        x: 50
        y: 375
        width: 502
        height: 61
        text: qsTr("ВНИМАНИЕ: если подключение уже существует, произойдет отключение и переподключение с новыми параметрами!")
        font.bold: false
        font.italic: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font.pixelSize: 15
    }
}
