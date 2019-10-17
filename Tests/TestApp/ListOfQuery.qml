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
                    onClicked: {
                        root.model.append({"comment": commentField.text})
                        root.model.commit()
                    }
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
                TextField {
                    id: idFiels
                    placeholderText: "Id"
                    text: model.id
                    onTextChanged: model.id = text
                }
                TextField {
                    id: comentField
                    placeholderText: "Coment"
                    text: model.comment
                    onTextChanged: model.comment = text
                }
                Row {
                    Button {
                        text: qsTr("Cancel")
                        onClicked: {
                            root.model.rollback()
                            changeRecord.close()
                        }
                    }
                    Button {
                        text: qsTr("OK")
                        onClicked: {
                            root.model.commit()
                            changeRecord.close()
                        }
                    }
                }
            }
        }
    }
}
