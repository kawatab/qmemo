// qMemo/main.qml - Application window
// qMemo is a note taking application
//
//  Copyright (C) 2019  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
//
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumWidth: 600
    minimumHeight: 300
    width: 800
    height: 600
    title: qsTr("qMemo")

    property alias fileListView: listPane.fileListView
    property alias textEdit: editPane.textEdit
    property bool textChanged: false
    property bool orderChanged: false
    property bool readyToSave: false
	
    RowLayout {
	id: layout
	anchors.fill: parent
	spacing: 0

	ListPane {
	    id: listPane
	    Layout.preferredWidth: 250
	    Layout.maximumWidth: 300
	    Layout.fillHeight: true

	    newButton.enabled: dataHandler.isEditable
	    fileListView.model: dataHandler.currentFileList

	    fileListView.onCurrentItemChanged: {
		var currentIndex = fileListView.currentIndex

		if (dataHandler.isEditable && dataHandler.hasCurrentFile()) {
		    if (textEdit.text.trim() == "") {
			var previousIndex = dataHandler.deleteEmptyFile()
			if (currentIndex > previousIndex) --currentIndex;
		    } else if (textChanged) {
			dataHandler.saveFileBeforeClosing(textEdit.text)
		    }
		}

		dataHandler.selectFile(currentIndex)
		textEdit.text = dataHandler.loadCurrentFile()
		textEdit.cursorPosition = 0
		textEdit.cursorVisible = true
		textEdit.focus = true
		textChanged = false
		readyToSave = false
	    }

	    fileListSelectBox.onCurrentIndexChanged: {
		if (fileListSelectBox.currentIndex == 0) {
		    dataHandler.isActiveMode = true
		} else {
		    if (textEdit.text.trim() == "") {
			dataHandler.deleteEmptyFile()
		    } else if (textChanged) {
			dataHandler.saveFileBeforeClosing(textEdit.text)
		    }

		    dataHandler.isActiveMode = false
		}

		if (fileListView.currentIndex < 0) {
		    dataHandler.releaseCurrentFile() // release before text clear
		    textEdit.text = ""
		    textChanged = false
		    readyToSave = false
		}

		textEdit.cursorPosition = 0
		textEdit.cursorVisible = true
		textEdit.focus = true
	    }

	    newButton.onClicked: {
		if (textEdit.text == "") return;

		if (dataHandler.createNewFile()) {
		    textChanged = false;
		    readyToSave = false;
		    fileListView.currentIndex = 0
		} else {
		    console.log("Failed to create a new file.")
		}
	    }

	    moveButton.onClicked: {
		if (dataHandler.isEditable && textEdit.text == "") return;

		if (textChanged) dataHandler.saveFileBeforeClosing(textEdit.text)
		dataHandler.moveCurrentFile(fileListView.currentIndex)
	    }
	}

	Rectangle {
	    Layout.preferredWidth: 1
	    Layout.fillHeight: true
	    color: "lightgray"
	}

	EditPane {
	    id: editPane
	    Layout.fillWidth: true
	    Layout.fillHeight: true
	    textEdit.readOnly: !dataHandler.isEditable
	    cutButton.enabled: dataHandler.isEditable
	    pasteButton.enabled: dataHandler.isEditable

	    textEdit.onTextChanged: {
		textChanged = true
	    }
	}
    }

    Timer {
	id: autoSaveTimer
	interval: 2000
	repeat: true

	onTriggered: {
	    if (dataHandler.isEditable) {
		if (textChanged) {
		    if (readyToSave) {
			var selectedIndex = fileListView.currentIndex

			if (dataHandler.saveCurrentFile(textEdit.text)) {
			    textChanged = false
			    readyToSave = false
			    console.log("Saved automatically.")
			} else {
			    return // do nothing if failed
			}
		    } else if (dataHandler.orderChanged) {
			dataHandler.orderChanged = false
		    }
		}

		readyToSave = textChanged
	    }
	}
    }

    Component.onCompleted: autoSaveTimer.start()

    onClosing: {
	if (textEdit.text.trim() == "") {
	    dataHandler.deleteEmptyFile()
	} else if (textChanged) {
	    dataHandler.saveFileBeforeClosing(textEdit.text)
	}
    }
}
