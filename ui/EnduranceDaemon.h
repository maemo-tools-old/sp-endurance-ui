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

#ifndef ENDURANCEDAEMON_H
#define ENDURANCEDAEMON_H

#include "endurancedaemon_interface.h"
#include <QObject>
#include <QDBusServiceWatcher>

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
	Q_PROPERTY(bool valid READ valid NOTIFY validChanged);

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
	bool tryShutdown();
	bool valid();
	void serviceRegistered(const QString &);
	void serviceUnregistered(const QString &);

signals:
	void collectionFailedChanged();
	void periodicCollectionActiveChanged();
	void snapshotIntervalInMinutesChanged();
	void takingSnapshotChanged();
	void nextCollectionTimestampChanged();
	void validChanged();
	void terminated();

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
	bool _valid : 1;
	int _snapshotIntervalInMinutes;
	QString _nextCollectionTimestamp;
	com::nokia::EnduranceDaemon _enduranceDaemon;
	QDBusServiceWatcher *_serviceWatcher;
};

#endif /* ENDURANCEDAEMON_H */
