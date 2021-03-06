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
import com.nokia.extras 1.0
import com.nokia.endurance 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
	id: report
	property EnduranceDirectoryModel enduranceDirectoryModel
	anchors.margins: UI.DEFAULT_MARGIN

	function ord(value) {
		var suffix = ["th", "st", "nd", "rd"]
		var digit = value % 10
		if (value > 3 && value < 21 || digit > 3) return "th"
		return suffix[digit]
	}

	Header {
		id: header
	}
	Column {
		anchors.topMargin: UI.DEFAULT_MARGIN
		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		spacing: 10

		ReportingControl {
			width: parent.width
			titleText: qsTr("Endurance Plot")
			firstSnapshot: firstSnapshot
			nextSnapshot: nextSnapshot
			reportControl: EndurancePlotControl {
				enduranceDirectoryModel: report.enduranceDirectoryModel
			}
			enduranceDirectoryModel: report.enduranceDirectoryModel
		}

		ReportingControl {
			width: parent.width
			titleText: qsTr("Endurance Report")
			firstSnapshot: firstSnapshot
			nextSnapshot: nextSnapshot
			reportControl: EnduranceReportControl {
				enduranceDirectoryModel: report.enduranceDirectoryModel
			}
			enduranceDirectoryModel: report.enduranceDirectoryModel
		}

		Label {
			text: qsTr("<h3>First snapshot:</h3>")
			width: parent.width
		}

		Label {
			text: qsTr("start with the %1%2 snapshot").arg(firstSnapshot.value).arg(report.ord(firstSnapshot.value))
			horizontalAlignment: Text.AlignHCenter
			width: parent.width
		}

		Slider {
			id: firstSnapshot
			minimumValue: 1
			maximumValue: enduranceDirectoryModel.rowCount < 2 ? 1 : enduranceDirectoryModel.rowCount - 1
			value: 1
			stepSize: 1
			valueIndicatorVisible: true
			width: parent.width
		}

		Label {
			text: qsTr("<h3>Snapshot frequency:</h3>")
			width: parent.width
		}

		Label {
			text: nextSnapshot.value == nextSnapshot.maximumValue ?
				qsTr("use the first and the last") :
				qsTr("use every %1%2").arg(nextSnapshot.value).arg(report.ord(nextSnapshot.value))
			horizontalAlignment: Text.AlignHCenter
			width: parent.width
		}

		Slider {
			id: nextSnapshot
			minimumValue: 1
			maximumValue: enduranceDirectoryModel.rowCount == 0 ? 1 :
					Math.ceil(((enduranceDirectoryModel.rowCount - firstSnapshot.value) / 2) + 1)
			value: 1
			stepSize: 1
			valueIndicatorVisible: true
			width: parent.width
		}

		Label {
			visible: enduranceDirectoryModel.rowCount >= 2
			text: {
				var snapshots;
				if (nextSnapshot.value == nextSnapshot.maximumValue)
					snapshots = 2;
				else
					snapshots = Math.ceil((enduranceDirectoryModel.rowCount
						- firstSnapshot.value) / nextSnapshot.value) + 1;
				return qsTr("Total number of snapshots used for plot and report generation: <b>%1</b>")
					.arg(snapshots);
			}
			width: parent.width
		}
	}
}
