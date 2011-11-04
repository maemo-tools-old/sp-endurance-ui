#ifndef ENDURANCECONTROL_H
#define ENDURANCECONTROL_H

#include "endurancedaemon_interface.h"
#include <QObject>

/* This is a proxy class for the DBus daemon. We cache the daemon state in
 * order to avoid unnecessary dbus traffic.
 */

class EnduranceDaemon : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool collectionFailed READ collectionFailed NOTIFY collectionFailedChanged)
	Q_PROPERTY(bool periodicCollectionActive READ periodicCollectionActive WRITE setPeriodicCollectionActive NOTIFY periodicCollectionActiveChanged)
	Q_PROPERTY(int snapshotIntervalInMinutes READ snapshotIntervalInMinutes WRITE setSnapshotIntervalInMinutes NOTIFY snapshotIntervalInMinutesChanged)
	Q_PROPERTY(bool takingSnapshot READ takingSnapshot NOTIFY takingSnapshotChanged)
	Q_PROPERTY(QString nextCollectionTimestamp READ nextCollectionTimestamp NOTIFY nextCollectionTimestampChanged)

public:
	EnduranceDaemon(QObject *parent = 0);

	bool collectionFailed() const;
	bool periodicCollectionActive() const;
	int snapshotIntervalInMinutes() const;
	bool takingSnapshot() const;
	QString nextCollectionTimestamp() const;

public slots:
	void takeSnapshot();
	void setSnapshotIntervalInMinutes(int);
	void setPeriodicCollectionActive(bool);

signals:
	void collectionFailedChanged();
	void periodicCollectionActiveChanged();
	void snapshotIntervalInMinutesChanged();
	void takingSnapshotChanged();
	void nextCollectionTimestampChanged();

private slots:
	void collectionFailedChangedSlot(bool);
	void periodicCollectionActiveChangedSlot(bool);
	void snapshotIntervalInMinutesChangedSlot(int);
	void takingSnapshotChangedSlot(bool);
	void nextCollectionTimestampChangedSlot(const QString &);

private:
	bool _collectionFailed : 1;
	bool _periodicCollectionActive : 1;
	bool _takingSnapshot : 1;
	int _snapshotIntervalInMinutes;
	QString _nextCollectionTimestamp;
	com::nokia::EnduranceDaemon _enduranceDaemon;
};

#endif /* ENDURANCECONTROL_H */
