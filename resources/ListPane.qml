// qMemo/ListPane.qml - List view pane
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

ListPaneForm {
    fileListView.onCountChanged: moveButton.enabled = fileListView.count > 0

    fileListView.delegate: ListItemForm {
	MouseArea {
	    anchors.fill: parent
	    onClicked: fileListView.currentIndex = index
	}
    }
}
