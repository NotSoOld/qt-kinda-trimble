import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls 2.2
import QtQml 2.2
import QtQuick.Layouts 1.3

// "Шаблон" для отображения информации об уровне сигнала спутников. Используется в главном окне.

Item {
    id: item1
    width: 250
    height: 40

    Text {
        id: mainLabel
        objectName: "mainLabel"
        y: 13
        text: "Спутник №0: 0 дБ, здоровье"
        anchors.left: parent.left
        anchors.leftMargin: 10
        renderType: Text.QtRendering
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
        width: 31
    }

    Image {
        id: sat_signal_image
        objectName: "sat_signal_image"
        x: 157
        y: 8
        anchors.right: parent.right
        anchors.rightMargin: 68
        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        transformOrigin: Item.Center
        source: "qrc:/images/satellites_levels/excellent.png"
        fillMode: Image.PreserveAspectFit
        sourceSize.height: 25
        sourceSize.width: 25

    }

}
