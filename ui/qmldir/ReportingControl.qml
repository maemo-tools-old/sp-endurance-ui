/*
 * This file is part of sp-endurance-ui package.
 *
 * Copyright (C) 2011 by Nokia Corporation
 *
 * Contact: Eero Tamminen <eero.tamminen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.endurance 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Column {
	property variant reportControl
	property EnduranceDirectoryModel enduranceDirectoryModel
	property alias titleText: titleLabel.text
	property Slider firstSnapshot
	property Slider nextSnapshot
	spacing: UI.PADDING_MEDIUM
	Label {
		id: titleLabel
		width: parent.width
		font.bold: true
		font.pixelSize: UI.FONT_SLARGE
	}
	Item {
		width: parent.width
		height: logButton.height
		Button {
			id: generateButton
			text: reportControl.generationInProgress ? qsTr("...") : qsTr("Generate")
			anchors.left: parent.left
			width: logButton.width ?
				parent.width/3 - (2*UI.BUTTON_SPACING)/3 :
				parent.width/2 - (UI.BUTTON_SPACING/2)
			enabled: !reportControl.generationInProgress && enduranceDirectoryModel.rowCount >= 2
			onClicked: reportControl.generate(firstSnapshot.value, nextSnapshot.value == nextSnapshot.maximumValue ?
					enduranceDirectoryModel.rowCount : nextSnapshot.value)
			Behavior on width { NumberAnimation { easing.type: Easing.OutBounce } }
		}
		Button {
			id: logButton
			anchors.horizontalCenter: parent.horizontalCenter
			text: qsTr("Log")
			width: (parent.width / 3 - 2) * (reportControl.log != "")
			enabled: reportControl.log != ""
			onClicked: logSheet.open()
			platformStyle: ButtonStyle {
				textColor: reportControl.generationError ? "red" : "black"
			}
			Behavior on width { NumberAnimation { duration: 500; easing.type: Easing.OutBounce} }
		}
		Button {
			id: viewButton
			anchors.right: parent.right
			text: qsTr("View")
			width: logButton.width ?
				parent.width/3 - (2*UI.BUTTON_SPACING)/3 :
				parent.width/2 - (UI.BUTTON_SPACING/2)
			enabled: reportControl.available
			onClicked: reportControl.open()
			Behavior on width { NumberAnimation { easing.type: Easing.OutBounce} }
		}
	}
	Item {
		width: parent.width
		height: 10

// TODO: enable the progressbar if it's performance gets reasonable.
/*
		ProgressBar {
			indeterminate: true
			width: parent.width
			visible: reportControl.generationInProgress
		}
*/
	}

	Sheet {
		id: logSheet
		acceptButtonText: qsTr("Clear")
		rejectButtonText: qsTr("Close")
		title: BusyIndicator {
			anchors.centerIn: parent
			running: reportControl.generationInProgress
			visible: reportControl.generationInProgress
		}
		content: Flickable {
			anchors.fill: parent
			anchors.leftMargin: 10
			anchors.topMargin: 10
			contentWidth: logContent.width
			contentHeight: logContent.height
			flickableDirection: Flickable.VerticalFlick
			Label {
				id: logContent
				text: reportControl.log
				textFormat: Text.RichText
				width: mainpage.width - 16
			}
		}
		onAccepted: reportControl.clearLog()
	}
}
