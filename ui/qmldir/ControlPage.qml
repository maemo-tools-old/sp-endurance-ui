import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.nokia.endurance 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
	id: root
	property EnduranceDaemon enduranceDaemon
	property EnduranceDirectoryModel enduranceDirectoryModel
	anchors.margins: UI.DEFAULT_MARGIN

	Header {
		id: header
	}

	SnapshotIntervalDialog {
		id: intervalSelectionDialog
		enduranceDaemon: root.enduranceDaemon
	}

	Column {
		anchors.topMargin: UI.DEFAULT_MARGIN
		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		spacing: 6

		ReportingControl {
			width: parent.width
			titleText: qsTr("Endurance Plot")
			reportControl: EndurancePlotControl {
				enduranceDirectoryModel: root.enduranceDirectoryModel
			}
		}

		ReportingControl {
			width: parent.width
			titleText: qsTr("Endurance Report")
			reportControl: EnduranceReportControl {
				enduranceDirectoryModel: root.enduranceDirectoryModel
			}
		}

		Label {
			text: "<h3>Periodic snapshotting:</h3>"
			width: parent.width
		}
		Button {
			text: enduranceDaemon.periodicCollectionActive ?
				qsTr("Stop periodic collection") : qsTr("Start periodic collection")
			width: parent.width
			onClicked: enduranceDaemon.setPeriodicCollectionActive(
				!enduranceDaemon.periodicCollectionActive)
		}
		Row {
			width: parent.width
			enabled: !enduranceDaemon.periodicCollectionActive
			spacing: UI.BUTTON_SPACING
			Button {
				text: enduranceDaemon.snapshotIntervalInMinutes >= 60 ?
					qsTr("Interval: %1h").arg(Math.floor(enduranceDaemon.snapshotIntervalInMinutes/60)) :
					qsTr("Interval: %1min").arg(enduranceDaemon.snapshotIntervalInMinutes)
				onClicked: intervalSelectionDialog.open()
				width: (2 * parent.width / 3) - 2
			}
			ButtonRow {
				width: (parent.width / 3) - 2
				checkedButton: enduranceDaemon.snapshotIntervalInMinutes >= 60 ? intervalHours : intervalMinutes
				Button {
					id: intervalMinutes
					text: qsTr("min")
					onClicked: {
						if (enduranceDaemon.snapshotIntervalInMinutes >= 60)
							enduranceDaemon.setSnapshotIntervalInMinutes(
								Math.floor(enduranceDaemon.snapshotIntervalInMinutes/60))
					}
				}
				Button {
					id: intervalHours
					text: qsTr("h")
					onClicked: {
						if (enduranceDaemon.snapshotIntervalInMinutes < 60)
							enduranceDaemon.setSnapshotIntervalInMinutes(
								enduranceDaemon.snapshotIntervalInMinutes*60)
					}
				}
			}
		}
		Item { width: parent.width; height: 10 }

		Label {
			text: "<h3>Manual snapshotting:</h3>"
			width: parent.width
		}
		Button {
			text: qsTr("Take one snapshot now")
			width: parent.width
			onClicked: enduranceDaemon.takeSnapshot()
			enabled: !enduranceDaemon.takingSnapshot
		}
		Item { width: parent.width; height: 10 }

		Label {
			text: "<h3>Snapshotting status:</h3>"
			width: parent.width
		}
		Label {
			text: {
				if (enduranceDaemon.takingSnapshot) {
					return qsTr("<font color='green'>Collecting snapshot ...</font>");
				} else if (enduranceDaemon.periodicCollectionActive) {
					return qsTr("<font color='green'>Next snapshot at %1.</font>")
						.arg(enduranceDaemon.nextCollectionTimestamp);
				} else {
					return qsTr("Idle.");
				}
			}
			width: parent.width
		}
	}
/*
	ScrollDecorator {
		flickableItem: snapshotsView
	}
*/
}
