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
