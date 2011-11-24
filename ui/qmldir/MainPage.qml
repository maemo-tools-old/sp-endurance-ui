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

Page {
	id: mainpage
	EnduranceDaemon {
		id: ed
	}
	EnduranceDirectoryModel {
		id: dirModel
	}
	DataArchiveSource {
		id: dataArchiveSource
		model: dirModel
	}
	ReportArchiveSource {
		id: reportArchiveSource
	}
	EnduranceArchiver {
		id: dataEnduranceArchiver
		source: dataArchiveSource
		outputPath: "/home/user/MyDocs/Documents/"
		outputTemplate: "sp-endurance-data"
	}
	EnduranceArchiver {
		id: reportEnduranceArchiver
		source: reportArchiveSource
		outputPath: "/home/user/MyDocs/Documents/"
		outputTemplate: "sp-endurance-report"
	}
	Menu {
		id: myMenu
		MenuLayout {
			MenuItem {
				text: qsTr("Remove collected snapshots")
				onClicked: dirModel.clearEnduranceData()
			}
			MenuItem {
				text: qsTr("Archive collected snapshots")
				onClicked: {
					archiveDialog.process(dataEnduranceArchiver)
				}
			}
			MenuItem {
				text: qsTr("Archive generated reports")
				onClicked: {
					archiveDialog.process(reportEnduranceArchiver)
				}
			}
			MenuItem {
				text: qsTr("View last snapshot log")
				onClicked: logSheet.open()
			}
			MenuItem {
				text: qsTr("About")
				onClicked: pageStack.push(helpPage)
			}
		}
	}

	tools: ToolBarLayout {
		ButtonRow {
			platformStyle: TabButtonStyle { }
			TabButton {
				text: qsTr("Control")
				tab: controlPage
			}
			TabButton {
				text: qsTr("Collected data")
				tab: dirPage
			}
		}
		ToolIcon {
			iconId: "toolbar-view-menu"
			onClicked: myMenu.open()
		}
	}

	TabGroup {
		currentTab: controlPage
		ControlPage {
			id: controlPage
			enduranceDaemon: ed
			enduranceDirectoryModel: dirModel
		}
		EnduranceDirectoryPage {
			id: dirPage
			enduranceDirectoryModel: dirModel
		}
	}

	HelpPage {
		id: helpPage
	}

	QueryDialog {
		id: archiveDialog
		rejectButtonText: _archiver.archiveInProgress ? qsTr("Cancel") : qsTr("")
		acceptButtonText: _archiver.archiveInProgress ? qsTr("") : qsTr("OK")
		message: _archiver.archiveInProgress ?
			qsTr("Archiving...") :
			_archiver.archiveError ?
				qsTr("Error creating archive:<br/>%1").arg(_archiver.log) :
				qsTr("Archive created:<br/><tt>MyDocs/Documents/" + _archiver.outputFilename + "</tt>");
		onAccepted: {
			_archiver.clearLog()
		}
		onRejected: {
			_archiver.abort()
			_archiver.clearLog()
		}

		property EnduranceArchiver _archiver
		
		function process(archiver) {
			_archiver = archiver
			open()
			_archiver.archive()
		}

	}
	
	Sheet {
		id: logSheet

		EnduranceLogReader {
			id: logReader
		}

		rejectButtonText: qsTr("Close")

		onStatusChanged: {
			if (status == DialogStatus.Opening) logReader.startMonitoring()
			if (status == DialogStatus.Closed) logReader.stopMonitoring()
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
				text: logReader.log
				textFormat: Text.RichText
				width: mainpage.width - 16
			}
		}
	}
}
