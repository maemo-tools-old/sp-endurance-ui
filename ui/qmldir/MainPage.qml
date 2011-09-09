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
	EnduranceArchiver {
		id: enduranceArchiver
		enduranceDirectoryModel: dirModel
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
					archiveDialog.open()
					enduranceArchiver.archive()
				}
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
		rejectButtonText: enduranceArchiver.archiveInProgress ? qsTr("Cancel") : qsTr("")
		acceptButtonText: enduranceArchiver.archiveInProgress ? qsTr("") : qsTr("OK")
		message: enduranceArchiver.archiveInProgress ?
			qsTr("Archiving...") :
			enduranceArchiver.archiveError ?
				qsTr("Error creating archive:<br/>%1").arg(enduranceArchiver.log) :
				qsTr("Snapshot archive created:<br/><tt>MyDocs/Documents/sp-endurance.zip</tt>");
		onAccepted: {
			enduranceArchiver.clearLog()
		}
		onRejected: {
			enduranceArchiver.abort()
			enduranceArchiver.clearLog()
		}
	}
}
