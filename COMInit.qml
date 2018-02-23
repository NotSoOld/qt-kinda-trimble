import QtQuick 2.1
import QtQuick.Controls 1.5
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

Window {
    id: comInitWindow

    visible: true
    width: 400
    height: 480
    title: "Настройка подключения"
    signal closeWindow()

    Text {
        id: text1
        x: 84
        y: 12
        text: qsTr("Настройка подключения\nпо VirtualCOM")
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
    }

    Text {
        id: text2
        x: 95
        y: 75
        text: qsTr("Имя порта:")
        font.pixelSize: 16
    }

    TextField {
        id: textEdit
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

    Text {
        id: text3
        x: 60
        y: 109
        text: qsTr("Скорость (бод):")
        font.pixelSize: 16
    }

    ComboBox {
        id: comboBox
        x: 211
        y: 99
        width: 90
        height: 40
        model: [1200, 9600]
    }

    Text {
        id: text4
        x: 76
        y: 143
        text: qsTr("Биты данных:")
        font.pixelSize: 16
    }

    ComboBox {
        id: comboBox1
        x: 211
        y: 133
        width: 90
        height: 40
    }

    Text {
        id: text5
        x: 106
        y: 178
        text: qsTr("Четность:")
        font.pixelSize: 16
    }

    ComboBox {
        id: comboBox2
        x: 211
        y: 168
        width: 90
        height: 40
    }

    Text {
        id: text6
        x: 46
        y: 211
        text: qsTr("Контроль потока:")
        font.pixelSize: 16
    }

    ComboBox {
        id: comboBox3
        x: 211
        y: 201
        width: 90
        height: 40
    }

    Text {
        id: text7
        x: 95
        y: 244
        text: qsTr("Стоп-биты:")
        font.pixelSize: 16
    }

    ComboBox {
        id: comboBox4
        x: 211
        y: 234
        width: 90
        height: 40
    }

    Button {
        id: button
        x: 124
        y: 309
        width: 153
        height: 51
        text: qsTr("Подключиться")
        onClicked: {
            comInitWindow.closeWindow()
        }
    }

    Text {
        id: text8
        x: 36
        y: 380
        width: 329
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
