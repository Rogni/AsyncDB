import QtQuick 2.12
import QtQuick.Controls 2.12
import AsyncDB 1.0

ListView {
    delegate: ItemDelegate {
        width: parent.width
        text: qsTr("id: %1, comment, %2").arg(model.id).arg(model.comment)
        onClicked: changeRecord.open()
        Popup {
            id: changeRecord
            parent: Overlay.over
            Column {
                ADBItemModel {
                    id: modelItem
                    model: adbModel
                    rowIndex: index
                    onChanged: {
                        idFiels.text = at("id")
                        comentField.text = at("comment")
                    }
                }
                
                TextField {
                    id: idFiels
                    placeholderText: "Id"
                }
                TextField {
                    id: comentField
                    placeholderText: "Coment"
                }
                Button {
                    text: qsTr("OK")
                    onClicked: {
                        modelItem.write("id", idFiels.text)
                        modelItem.write("comment", comentField.text)
                        modelItem.commit()
                        changeRecord.close()
                    }
                }
            }
        }
    }
}