import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls 2.2
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Window {
    id: window
    visible: true
    width: 800
    height: 600
    color: "#ffffff"
    opacity: 1
    title: qsTr("Kinda Trimble")

    signal sig_send_COMMAND_REQUEST_STATUS_AND_POS()
    signal sig_send_COMMAND_SET_IO_OPTIONS()

    function onAppendReceivedtext(s) {
        receivedText.append(s+"\n\n------------/-/-/-/-/------------\n")
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
            text: ""
            textFormat: Text.AutoText
            font.weight: Font.Light
            readOnly: false
            font.wordSpacing: 0
            wrapMode: Text.NoWrap
            font.pixelSize: 14

        }
    }

    TabBar {
        id: tabs
        x: 18
        y: 25
        width: 400
        height: 40
        clip: true

        TabButton {
            id: essentialsTab
            text: "Основное"
            width: implicitWidth + 15
            //source: "essentials.qml"
        }

        TabButton {
            id: satelliteInfo
            text: "Информация о спутниках"
            width: implicitWidth + 15
           //source: "satellite_info.qml"
        }

        TabButton {
            id: ioOptions
            text: "Опции ввода/вывода"
            width: implicitWidth + 15
            //source: "io_options.qml"
        }

        TabButton {
            id: initPosition
            text: "Начальная позиция"
            width: implicitWidth + 15
            //source: "init_position.qml"
        }

        TabButton {
            id: otherStuff
            text: "Включение и здоровье спутников"
            width: implicitWidth + 15
            //source: "other_stuff.qml"
        }

        onCurrentIndexChanged: {
            switch (currentIndex)
            {
            case 0:
                loader.source = "qrc:/essentials.qml"
                break;
            case 1:
                loader.source = "qrc:/satellite_info.qml"
                break;
            case 2:
                loader.source = "qrc:/io_options.qml"
                break;
            case 3:
                loader.source = "qrc:/init_position.qml"
                break;
            case 4:
                loader.source = "qrc:/other_stuff.qml"
                break;
            }
        }

        /*
        style: TabViewStyle {
            tab: Rectangle {
                color: styleData.selected ? "white" : "silver"
                border.color: "darkgrey"
                implicitWidth: mytext.width + 15
                implicitHeight: 30
                radius: 2
                Text {
                    id: mytext
                    anchors.centerIn: parent
                    text: styleData.title
                    color: "black"
                }
            }
            frame: Rectangle {
                color: "steelblue"
            }
        }
        */
    }

    Loader {
        id: loader
        x: 18
        y: 65
        source: "qrc:/essentials.qml"
    }

}
