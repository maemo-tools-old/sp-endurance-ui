import QtQuick 1.1
import com.nokia.meego 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Rectangle {
	property alias text: label.text
	signal refresh
	//anchors.left: parent.left
	//anchors.right: parent.right
	width: parent.width + 32
	height: UI.HEADER_DEFAULT_HEIGHT_PORTRAIT
	clip: true
	color: "#007000"
	z: 10
	x: -16
	y: -16
	Label {
		id: label
		text: "sp-endurance"
		width: parent.width - 2*UI.DEFAULT_MARGIN
		font.pixelSize: UI.FONT_XLARGE
		color: "white"
		z: parent.z + 1
		y: UI.DEFAULT_MARGIN
		x: UI.DEFAULT_MARGIN
	}
}
