// qMemo/EditPaneForm.ui.qml - Text edit pane UI
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

Item {
    property alias flick: flick
    property alias textEdit: textEdit
    property alias selectAllButton: selectAllButton
    property alias cutButton: cutButton
    property alias copyButton: copyButton
    property alias pasteButton: pasteButton

    ColumnLayout {
	anchors.fill: parent
	spacing: 4

	GridLayout {
	    Layout.alignment: Qt.AlignTop
	    Layout.preferredHeight: 32
	    Layout.maximumHeight: 32
	    Layout.fillWidth: true
	    Layout.topMargin: 4
	    Layout.leftMargin: 4
	    Layout.rightMargin: 4
	    flow: GridLayout.LeftToRight
	    columnSpacing: 4
	    columns: 4

	    Button {
		id: selectAllButton
		Layout.fillHeight: true
		Layout.maximumWidth: 120
		Layout.fillWidth: true
		text: qsTr("Select All")
		Layout.leftMargin: 4
	    }

	    Button {
		id: cutButton
		Layout.fillHeight: true
		Layout.maximumWidth: 120
		Layout.fillWidth: true
		text: qsTr("Cut")
	    }

	    Button {
		id: copyButton
		Layout.fillHeight: true
		Layout.maximumWidth: 120
		Layout.fillWidth: true
		text: qsTr("Copy")
	    }

	    Button {
		id: pasteButton
		Layout.fillHeight: true
		Layout.maximumWidth: 120
		Layout.fillWidth: true
		text: qsTr("Paste")
	    }
	}
	
	Rectangle {
	    Layout.preferredHeight: 1
	    Layout.fillWidth: true
	    color: "lightgray"
	}

	Flickable {
	    id: flick
	    Layout.alignment: Qt.AlignBottom
	    Layout.fillWidth: true
	    Layout.fillHeight: true
	    Layout.margins: 4
	    // contentWidth: textEdit.paintedWidth
	    contentHeight: textEdit.paintedHeight
	    clip: true

	    ScrollBar.vertical: ScrollBar {
		contentItem.opacity: 1 // need this under 5.9
	    }

	    TextEdit {
		id: textEdit
		Layout.fillWidth: true
		Layout.fillHeight: true
		// width: parent.width
		// height: parent.height
		selectByMouse: true
		selectByKeyboard: true
		font.pointSize: 10
		textFormat: TextEdit.PlainText
		wrapMode: TextEdit.Wrap
	    }
	}
    }
}
