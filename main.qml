import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQml 2.2

Window {
    id: window
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

    Rectangle {
        id: rectangle
        x: 450
        y: 23
        width: 334
        height: 542
        color: "#00000000"
        border.color: "#a9a9a9"

        TextArea {
            id: receivedText
            x: 1
            y: 1
            width: 332
            height: 540
            font.weight: Font.Light
            readOnly: false
            font.wordSpacing: -4
            wrapMode: Text.NoWrap
            font.pixelSize: 14
        }
    }

    TabView {
        id: tabView
        x: 18
        y: 35
        width: 400
        height: 517
        currentIndex: 0

        Tab {
            id: essentialsTab
            title: "Основное"
            source: "essentials.qml"
        }

        Tab {
            id: satellite_info
            title: "Информация о спутниках"
            source: "satellite_info.qml"
        }

        Tab {
            id: io_options
            title: "Опции ввода/вывода"
            source: "io_options.qml"
        }

        Tab {
            id: init_position
            title: "Начальная позиция"
            source: "init_position.qml"
        }

        Tab {
            id: other_stuff
            title: "Включение и здоровье спутников"
            source: "other_stuff.qml"
        }

    }

}
