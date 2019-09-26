import QtQuick 2.12
import QtQuick.Controls 2.12
import AsyncDB 1.0

ApplicationWindow {
    id: root
    width: 300
    height: 400
    visible: true

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
}
