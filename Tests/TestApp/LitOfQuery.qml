import QtQuick 2.12
import QtQuick.Controls 2.12
import AsyncDB 1.0

ListView {
    id: root
    header: ItemDelegate {
        width: parent.width
        text: qsTr("Append new")
        onClicked: appendPopup.open()
        Popup {
            parent: overlay
            id: appendPopup
            anchors.centerIn: parent
            Column {
                TextField {
                    id: commentField
                    placeholderText: qsTr("Comment")
                }
                Button {
                    text: qsTr("Ok")
                    onClicked: root.model.append({"comment": commentField.text})
                }
            }

        }
    }

    delegate: ItemDelegate {
        width: parent.width
        text: qsTr("id: %1, comment, %2").arg(model.id).arg(model.comment)
        onClicked: changeRecord.open()
        Popup {
            id: changeRecord
            parent: Overlay.overlay
            anchors.centerIn: parent
            Column {
                ADBItemModel {
                    id: modelItem
                    model: adbModel
                    rowIndex: index
                    onChanged: {
                        idFiels.text = at("id") ? at("id") : ""
                        comentField.text = at("comment") ? at("comment"): ""
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
