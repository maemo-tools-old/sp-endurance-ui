import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.nokia.endurance 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
	property EnduranceDirectoryModel enduranceDirectoryModel
	anchors.margins: UI.DEFAULT_MARGIN
	tools: ToolBarLayout {
		ToolIcon {
			iconId: "toolbar-back"
			onClicked: pageStack.pop()
		}
	}
	Header {
		id: header
	}
	Column {
		anchors.topMargin: UI.DEFAULT_MARGIN
		anchors.top: header.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		spacing: UI.DEFAULT_MARGIN
		Label {
			width: parent.width
			text: qsTr("sp-endurance is a tool for discovering "
				+ "memory and other resource leaks in "
				+ "applications. Collect endurance data snapshots "
				+ "over long periods of time while repeating the "
				+ "use case under analysis. Once collected, "
				+ "generate the HTML endurance report and plots, "
				+ "and look for growth patterns.");
		}
		Label {
			width: parent.width
			text: qsTr("<b>Note:</b> periodic snapshot collection "
				+ "continues in the background after closing this "
				+ "application, and is also automatically restarted after "
				+ "device is rebooted.")
		}
	}
}
