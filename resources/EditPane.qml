// qMemo/EditPane.qml - Text edit pane
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

EditPaneForm {
    Keys.onPressed: {
	if(event.key == Qt.Key_PageDown) {
	    textEdit.cursorPosition = textEdit.positionAt(0, textEdit.cursorRectangle.y + flick.height)
	}
	else if(event.key == Qt.Key_PageUp) {
	    textEdit.cursorPosition = textEdit.positionAt(0, textEdit.cursorRectangle.y - flick.height)
	}
    }

    selectAllButton.onClicked: {
	selectAllButton.focus = false
	textEdit.selectAll()
    }
    
    cutButton.onClicked: {
	cutButton.focus = false
	textEdit.cut()
    }
    
    copyButton.onClicked: {
	copyButton.focus = false
	textEdit.copy()
    }
    
    pasteButton.onClicked: {
	pasteButton.focus = false
	textEdit.paste()
    }
    


    textEdit.onCursorRectangleChanged: ensureVisible(textEdit.cursorRectangle)

    function ensureVisible(r) {
	if (flick.contentY >= r.y)
	    flick.contentY = r.y;
	else if (flick.contentY+height <= r.y+r.height)
	    flick.contentY = r.y+r.height-height;
    }
}
