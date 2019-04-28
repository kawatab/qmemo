// qMemo/ListItemForm.ui.qml - Item UI of list view
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
    height: 65
    width: parent.width
    id: wrapper

    ColumnLayout {
	anchors.fill: parent
	width: parent.width
	spacing: 2

	Text {
	    Layout.fillWidth: true
	    Layout.preferredHeight: 37
	    Layout.maximumHeight: 37
	    maximumLineCount: 2
	    elide: Text.ElideRight
	    wrapMode: Text.Wrap
	    font.pointSize: 10
	    text: preview
	}

	Label {
	    Layout.fillWidth: true
	    Layout.preferredHeight: 18
	    Layout.maximumHeight: 18
	    wrapMode: Text.Wrap
	    font.pointSize: 8
	    font.bold: true
	    text: modified
	}

	Rectangle {
	    Layout.fillWidth: true
	    Layout.preferredHeight: 1
	    color: "lightgrey"
	}
    }
}
