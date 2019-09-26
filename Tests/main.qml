import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import AsyncDB 1.0

ApplicationWindow {
    id: root
    width: 300
    height: 400
    visible: true
    title: "Custom Config"
    ADBDatabase {
        id: testDatabase
        configuration: ADBDatabaseConfiguration {
            type: "QSQLITE"
            databaseName: "TestsDB.sqlite"
        }
    }

    CustomConfigPage {
        anchors.fill: parent
        database: testDatabase
    }

    Window {
        width: 300
        height: 400
        visible: true
        title: "List Config"

        ListConfigurationPage {
            anchors.fill: parent
            database: testDatabase
        }
    }


}
