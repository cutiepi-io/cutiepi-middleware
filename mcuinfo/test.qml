import QtQuick 2.0
import com.mycompany.qmlcomponents 1.0

Rectangle {
    width: 500; height: 200
    color: "lightgray"

    Text {
        id: batteryInfo
        text: mcuInfo.battery
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }

    McuInfo{
        id: mcuInfo
        portName: "/dev/ttyS0"
        portBaudRate: 115200
    }

    Component.onCompleted: {
        mcuInfo.start();
    }
}