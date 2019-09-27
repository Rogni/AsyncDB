import QtQuick 2.12
import QtQuick.Controls 2.12
import AsyncDB 1.0

Page {
    id: root
    property ADBDatabase database

    ADBListModel {
        id: adbModel

        configuration: ADBCustomListModelConfiguration {
            selectQuery: "SELECT id, comment FROM TestTable WHERE id > 2"
            database: root.database
        }
    }

    LitOfQuery {
        anchors.fill: parent
        model: adbModel
    }
}
