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

SelectionDialog {
	id: intervalDialog
	property EnduranceDaemon enduranceDaemon
	titleText: qsTr("Snapshotting interval")
	ListModel {
		id: intervalModel
		ListElement { name:  "1" }
		ListElement { name:  "2" }
		ListElement { name:  "3" }
		ListElement { name:  "4" }
		ListElement { name:  "5" }
		ListElement { name:  "6" }
		ListElement { name:  "7" }
		ListElement { name:  "8" }
		ListElement { name:  "9" }
		ListElement { name: "10" }
		ListElement { name: "11" }
		ListElement { name: "12" }
		ListElement { name: "13" }
		ListElement { name: "14" }
		ListElement { name: "15" }
		ListElement { name: "16" }
		ListElement { name: "17" }
		ListElement { name: "18" }
		ListElement { name: "19" }
		ListElement { name: "20" }
		ListElement { name: "21" }
		ListElement { name: "22" }
		ListElement { name: "23" }
		ListElement { name: "24" }
		ListElement { name: "25" }
		ListElement { name: "26" }
		ListElement { name: "27" }
		ListElement { name: "28" }
		ListElement { name: "29" }
		ListElement { name: "30" }
		ListElement { name: "31" }
		ListElement { name: "32" }
		ListElement { name: "33" }
		ListElement { name: "34" }
		ListElement { name: "35" }
		ListElement { name: "40" }
		ListElement { name: "45" }
		ListElement { name: "50" }
		ListElement { name: "55" }
	}
	function updateSelectedIndex() {
		var newSelectedIndex = 0
		var interval = enduranceDaemon.snapshotIntervalInMinutes;
		if (interval >= 60)
			interval = interval/60;
		for (var i=0; i < intervalModel.count; ++i) {
			if (intervalModel.get(i).name == ""+interval) {
				newSelectedIndex = i;
				break;
			}
		}
		intervalDialog.selectedIndex = newSelectedIndex;
	}
	onAccepted: {
		var newInterval = intervalModel.get(intervalDialog.selectedIndex).name;
		if (enduranceDaemon.snapshotIntervalInMinutes >= 60)
			newInterval *= 60;
		enduranceDaemon.setSnapshotIntervalInMinutes(newInterval);
	}
	model: intervalModel
	Connections {
		target: enduranceDaemon
		onSnapshotIntervalInMinutesChanged: updateSelectedIndex();
	}
	Component.onCompleted: updateSelectedIndex();
}
