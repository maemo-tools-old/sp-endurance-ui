import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.nokia.endurance 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
	property EnduranceDirectoryModel enduranceDirectoryModel
	anchors.margins: UI.DEFAULT_MARGIN
	Header {
		id: header
	}
	ListView {
		id: dirView
		anchors.top: header.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		model: enduranceDirectoryModel
		delegate: ListDelegate {
			height: UI.LIST_ITEM_HEIGHT_SMALL
		}
		footer: Label {
			text: qsTr("Total size: %1 MB")
				.arg(enduranceDirectoryModel.totalSizeMB)
			width: parent.width
			font.pixelSize: UI.FONT_SLARGE
		}
	}
	ScrollDecorator {
		flickableItem: dirView
	}
}
