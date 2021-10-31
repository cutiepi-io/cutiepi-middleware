import QtQuick 2.0
import Nemo.DBus 2.0

Item {
    width: 400
    height: 300

    DBusInterface {
        id: mcuProxy

        service: 'io.cutiepi.service'
        iface: 'io.cutiepi.interface'
        path: '/mcu'

        signalsEnabled: true

        function updateEvent(type, value) {
            console.log(type +' value: ' + value); 
        }
    }
}
