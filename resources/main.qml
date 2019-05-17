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


import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

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
		if (fileListView.count == 0) {
		    clearTextEdit()
		    return
		}
		
		var index = fileListView.currentIndex

		if (dataHandler.matchCurrentFile(index)) return
		
		if (dataHandler.isEditable && dataHandler.hasCurrentFile()) {
		    if (textEdit.text.trim() == "") {
			index = dataHandler.deleteEmptyFile(index)
		    } else if (textChanged) {
			index = dataHandler.saveCurrentFile(textEdit.text, index)
			clearTextEdit()
		    }
		}
		console.debug("currentItemChanged")

		dataHandler.selectFile(index)
		fileListView.currentIndex = index // invoke recursively
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
		    if (dataHandler.hasCurrentFile()) {
			if (textEdit.text.trim() == "") {
			    dataHandler.deleteEmptyFile()
			} else if (textChanged) {
			    dataHandler.saveCurrentFile(textEdit.text)
			}
		    }

		    dataHandler.isActiveMode = false
		}

		if (fileListView.count > 0) {
		    fileListView.currentIndex = 0
		}
	    }

	    newButton.onClicked: {
		if (textEdit.text == "" && dataHandler.hasCurrentFile()) return;
		var index = dataHandler.createNewFile("")

		if (index < 0) {
		    console.error("Failed to create a new file.")
		} else {
		    fileListView.currentIndex = index
		}
	    }

	    moveButton.onClicked: {
		var index = fileListView.currentIndex
		
		if (dataHandler.isEditable) {
		    if(textEdit.text == "") return;

		    if (textChanged) {
			index = dataHandler.saveCurrentFile(textEdit.text, index)
		    }
		}

		textChanged = false // avoid to save wrongly
		dataHandler.moveCurrentFile(index)

		if (fileListView.count == 0) {
		    clearTextEdit()
		}
	    }

	    function clearTextEdit() {
		dataHandler.releaseCurrentFile()
		textEdit.text = ""
		textEdit.focus = true
		textChanged = false
		readyToSave = false
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
		readyToSave = false
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
			if (dataHandler.hasCurrentFile()) {
			    var index = dataHandler.saveCurrentFile(textEdit.text, fileListView.currentIndex)

			    if (index < 0) {
				console.error("Failed to save file: MainWindow::autoSave()");
				return // do nothing if failed
			    } else {
				textChanged = false
				readyToSave = false
				console.debug("Saved automatically.: main.qml::autoSaveTimer")
				fileListView.currentIndex = index
			    }
			} else {
			    var index = dataHandler.createNewFile(textEdit.text)
			    if (index < 0) {
				console.error("Failed to create a new file.: main.qml::autoSaveTimer")
			    } else {
				fileListView.currentIndex = index
				console.debug("create new file.: main.qml::autoSaveTimer")
			    }
			}
		    }
		}

		readyToSave = textChanged
	    }
	}
    }

    Component.onCompleted: {
	listPane.fileListSelectBox.currentIndex = 1
	listPane.fileListSelectBox.currentIndex = 0

	autoSaveTimer.start()
    }

    onClosing: {
	if (dataHandler.isEditable && dataHandler.hasCurrentFile()) {
	    if (textEdit.text.trim() == "") {
		dataHandler.deleteEmptyFile()
	    } else if (textChanged) {
		dataHandler.saveCurrentFile(textEdit.text)
		dataHandler.releaseCurrentFile()
	    }
	}
    }
}
