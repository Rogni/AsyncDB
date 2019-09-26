import QtQuick 2.12
import QtQuick.Controls 2.12
import AsyncDB 1.0

Page {
    id: root
    property ADBDatabase database

    ADBListModel {
        id: adbModel

        configuration: ADBListModelConfiguration {
            tablename: "TestTable"
            columns: ["id", "comment"]
            database: root.database
        }
    }

    LitOfQuery {
        anchors.fill: parent
        model: adbModel
    }
}
