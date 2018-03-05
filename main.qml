import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQml 2.2
import QtCharts 2.2
import "./" as MyQML

Window {
    id: main_window
    visible: true
    width: 1000
    height: 1000
    color: "#ffffff"
    opacity: 1
    title: qsTr("Kinda Trimble")

    property int _COMMAND_FIRMWARE_INFO                          : 0x1C
    property int     _CMDSUB_FIRMWARE_VERSION                    : 0x01
    property int     _CMDSUB_HARDWARE_COMPONENT_INFO             : 0x03
    property int _COMMAND_INITIATE_RESET                         : 0x1E
    property int _COMMAND_REQEST_SOFTWARE_VERSION                : 0x1F
    property int _COMMAND_REQUEST_GPS_SATELLITES                 : 0x24
    property int _COMMAND_INITIATE_HOT_RESET                     : 0x25
    property int _COMMAND_REQUEST_SIGNAL_LEVELS                  : 0x27
    property int _COMMAND_ACCURATE_INIT_POS_XYZ                  : 0x31
    property int _COMMAND_ACCURATE_INIT_POS_LLA                  : 0x32
    property int _COMMAND_SATELLITE_SELECTION                    : 0x34
    property int _COMMAND_SET_IO_OPTIONS                         : 0x35
    property int _COMMAND_REQUEST_STATUS_AND_POS                 : 0x37
    property int _COMMAND_REQUEST_SATELLITE_SYSTEM_DATA          : 0x38
    property int _COMMAND_SET_REQUEST_SATELLITES_AND_HEALTH      : 0x39
    property int _COMMAND_REQUEST_LAST_RAW_MEASUREMENT           : 0x3A
    property int _COMMAND_REQUEST_SATELLITE_TRACKING_STATUS      : 0x3C
    property int _COMMAND_SET_RECEIVER_CONFIG                    : 0xBB
    property int _COMMAND_SET_PORT_CONFIG                        : 0xBC
    property int _COMMAND_SUPER                                  : 0x8E
    property int     _CMDSUB_REQUEST_CURRENT_DATUM               : 0x15
    property int     _CMDSUB_WRITE_CONFIG_TO_FLASH               : 0x26
    property int     _CMDSUB_REQUEST_MANUFACT_PARAMS             : 0x41
    property int     _CMDSUB_REQUEST_STORED_PRODUCTION_PARAMS    : 0x42
    property int     _CMDSUB_SET_PPS_CHARS                       : 0x4A
    property int     _CMDSUB_SET_PPS_OUTPUT                      : 0x4E
    property int     _CMDSUB_SET_DAC                             : 0xA0
    property int     _CMDSUB_SET_UTC_GPS_TIMIMG                  : 0xA2
    property int     _CMDSUB_ISSUE_OSC_DISCIPL_CMD               : 0xA3
    property int     _CMDSUB_TEST_MODES                          : 0xA4
    property int     _CMDSUB_SET_PACKET_BROADCAST_MASK           : 0xA5
    property int     _CMDSUB_ISSUE_SELF_SURVEY                   : 0xA6
    property int     _CMDSUB_SET_REQUEST_DISCIPL_PARAMS          : 0xA8
    property int     _CMDSUB_SET_SELF_SURVEY_PARAMS              : 0xA9
    property int     _CMDSUB_REQUEST_PRIMARY_TIMING_PACKET       : 0xAB
    property int     _CMDSUB_REQUEST_SUPPL_TIMING_PACKET         : 0xAC

    signal sig_send_command(int code, int subcode)
    signal sig_open_port(string portName, int baud, int dataBits, int parity, int flowControl, int stopBits)

    function onAppendReceivedtext(s) {
        receivedText.append(s+"\n\n--------------------------\n")
    }
/*
    function onUpdateSatelliteInfo(index, sat_no, sat_lvl) {
        var obj;
        if (index === 1) obj = template1;
        else if (index === 2) obj = template2;
        else if (index === 3) obj = template3;
        else if (index === 4) obj = template4;
        else if (index === 5) obj = template5;
        else if (index === 6) obj = template6;
        else if (index === 7) obj = template7;
        else if (index === 8) obj = template8;
        else if (index === 9) obj = template9;
        else if (index === 10) obj = template10;
        else if (index === 11) obj = template11;
        else if (index === 12) obj = template12;
        template1.visible = false;
        template2.visible = false;
        template3.visible = false;
        template4.visible = false;
        template5.visible = false;
        template6.visible = false;
        template7.visible = false;
        template8.visible = false;
        template9.visible = false;
        template10.visible = false;
        template11.visible = false;
        template12.visible = false;
        obj.visible = true;

    }
*/
    Rectangle {
        id: rectangle
        x: 445
        y: 65
        width: 521
        height: 904
        color: "#00000000"
        border.color: "#a9a9a9"

        ScrollView {
            id: view
            contentHeight: 30
            anchors.topMargin: 8
            anchors.leftMargin: 8
            anchors.fill: parent

            TextArea {
                id: receivedText
                x: -5
                y: -1
                width: 314
                height: 487
                text: ""
                wrapMode: Text.WordWrap
                font.weight: Font.Light
                readOnly: true
                font.wordSpacing: 0
                font.pixelSize: 14
            }
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

        TabButton {
            id: autopacketMasking
            text: "Управление авторассылкой"
            width: implicitWidth + 15
            //source: "autopacket_masking.qml"
        }

        TabButton {
            id: timingPackets
            text: "Пакеты по таймингу"
            width: implicitWidth + 15
            //source: "timing_packets.qml"
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
            case 5:
                loader.source = "qrc:/autopacket_masking.qml"
                break;
            case 6:
                loader.source = "qrc:/timing_packets.qml"
                break;
            }
        }
    }

    Loader {
        id: loader
        x: 18
        y: 65
        source: "qrc:/essentials.qml"
    }

    Text {
        id: text1
        x: 455
        y: 42
        text: qsTr("Лог ресивера")
        font.italic: false
        font.bold: true
        textFormat: Text.RichText
        font.pixelSize: 14
    }

    COMInit {
        id: com_init_window
        objectName: "com_init_window"
        modality: Qt.WindowModal

        onCloseWindow: {
            com_init_window.close();
            main_window.show();
            sig_open_port(portName, baud, dataBits, parity, flowControl, stopBits);
        }
    }

    Button {
        id: button
        x: 583
        y: 9
        width: 201
        height: 50
        text: qsTr("Настройка VirtualCOM")

        onClicked: {
            com_init_window.show();
            main_window.hide();
        }
    }

    Label {
        id: temperatureLabel
        objectName: "temperatureLabel"
        x: 24
        y: 591
        text: "Температура, С:"
    }

    Label {
        id: latitudeOutLabel
        objectName: "latitudeOutLabel"
        x: 24
        y: 611
        text: "Широта:"
    }

    Label {
        id: longitudeOutLabel
        objectName: "longitudeOutLabel"
        x: 24
        y: 631
        text: "Долгота:"
    }

    Label {
        id: altitudeOutLabel
        objectName: "altitudeOutLabel"
        x: 24
        y: 651
        text: "Высота, м:"
    }

    Text {
        id: text2
        x: 30
        y: 694
        text: qsTr("Спутники и уровни их сигнала")
        textFormat: Text.RichText
        font.pixelSize: 14
        font.italic: false
        font.bold: true
    }

    MyQML.Satellite_status_template {
        id: template1
        objectName: "template1"
        x: 24
        y: 720
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template2
        objectName: "template2"
        x: 24
        y: 740
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template3
        objectName: "template3"
        x: 24
        y: 760
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template4
        objectName: "template4"
        x: 24
        y: 780
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template5
        objectName: "template5"
        x: 24
        y: 800
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template6
        objectName: "template6"
        x: 24
        y: 820
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template7
        objectName: "template7"
        x: 24
        y: 840
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template8
        objectName: "template8"
        x: 24
        y: 860
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template9
        objectName: "template9"
        x: 24
        y: 880
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template10
        objectName: "template10"
        x: 24
        y: 900
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template11
        objectName: "template11"
        x: 24
        y: 920
        visible: false
    }

    MyQML.Satellite_status_template {
        id: template12
        objectName: "template12"
        x: 24
        y: 940
        visible: false
    }
/*
    Flickable {
        x: 100
        y: 650
        width: 300
        height: 300
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: myChart.height
        contentWidth: myChart.width
        clip: true
/*
        Image {
            id: img
            width: 535
            height: 492
            source: "./images/satellites_levels/excellent.png"
            fillMode: Image.PreserveAspectFit
        }*/

        ChartView {
            id: myChart
            x: 0
            y: 0
            width: 500
            height: 500

            MouseArea {
                anchors.fill: parent
                property int lastX: 0
                property int lastY: 0
                onPressed: {
                    lastX = mouse.x
                    lastY = mouse.y
                }

                onPositionChanged: {
                    if (lastX !== mouse.x) {
                        myChart.scrollRight(lastX - mouse.x)
                        lastX = mouse.x
                    }
                    if (lastY !== mouse.y) {
                        myChart.scrollDown(lastY - mouse.y)
                        lastY = mouse.y
                    }
                }
            }

            SplineSeries {
           // LineSeries {
                XYPoint { x: 0; y: 0 }
                XYPoint { x: 3; y: 3 }
                XYPoint { x: -3; y: 4 }
                XYPoint { x: -3; y: -4 }
                XYPoint { x: -2; y: 4 }
            }
        }
    //}
}
