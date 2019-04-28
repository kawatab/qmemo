// qMemo/ListPaneForm.ui.qml - List view pane UI
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

Item {
    property alias fileListView: fileListView
    property alias fileListSelectBox: fileListSelectBox
    property alias newButton: newButton
    property alias moveButton: moveButton

    ColumnLayout {
	anchors.fill: parent
	spacing: 4

	RowLayout {
	    Layout.alignment: Qt.AlignTop
	    Layout.preferredHeight: 32
	    Layout.maximumHeight: 32
	    Layout.fillWidth: true
	    Layout.topMargin: 4
	    spacing: 4

	    ComboBox {
		id: fileListSelectBox
		Layout.preferredWidth: 100
		Layout.fillHeight: true
		Layout.leftMargin: 4
		model: [ qsTr("Active"), qsTr("Archive") ]
	    }

	    Button {
		id: newButton
		Layout.preferredWidth: 60
		Layout.fillHeight: true
		text: qsTr("New")
	    }

	    Button {
		id: moveButton
		Layout.preferredWidth: 60
		Layout.fillHeight: true
		text: qsTr("Move")
	    }
	}
	
	Rectangle {
	    Layout.preferredHeight: 1
	    Layout.fillWidth: true
	    color: "lightgray"
	}

	ListView {
	    id: fileListView
	    Layout.alignment: Qt.AlignBottom
	    Layout.fillWidth: true
	    Layout.fillHeight: true
	    Layout.margins: 4
	    highlightMoveVelocity: -1
	    spacing: 0 // 4
	    clip: true

	    ScrollBar.vertical: ScrollBar {
		contentItem.opacity: 1 // need this under 5.9
	    }

	    property alias fileListView: fileListView

	    SystemPalette {
		id: myPalette
		colorGroup: SystemPalette.Active
	    }

	    highlight: Rectangle {
		width: fileListView.width
		id: wrapper
		color: myPalette.highlight
	    }
	}
    }
}
