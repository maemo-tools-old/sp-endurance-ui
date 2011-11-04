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
