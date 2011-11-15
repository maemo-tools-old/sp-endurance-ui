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

#include "Settings.h"
#include "SnapshotterLog.h"
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QDateTime>

class EnduranceDaemon: public QObject
{
	Q_OBJECT
public:
	EnduranceDaemon(QObject *parent = 0);
	~EnduranceDaemon() {}

public:
	Q_PROPERTY(bool collectionFailed READ collectionFailed)
	bool collectionFailed() const;

	Q_PROPERTY(bool periodicCollectionActive READ periodicCollectionActive WRITE setPeriodicCollectionActive)
	bool periodicCollectionActive() const;
	void setPeriodicCollectionActive(bool value);

	Q_PROPERTY(int snapshotIntervalInMinutes READ snapshotIntervalInMinutes WRITE setSnapshotIntervalInMinutes)
	int snapshotIntervalInMinutes() const;
	void setSnapshotIntervalInMinutes(int value);

	Q_PROPERTY(bool takingSnapshot READ takingSnapshot)
	bool takingSnapshot() const;

	Q_PROPERTY(QString nextCollectionTimestamp READ nextCollectionTimestamp NOTIFY nextCollectionTimestampChanged)
	QString nextCollectionTimestamp() const;

public slots:
	void takeSnapshot();
	bool tryShutdown();

signals:
	void collectionFailedChanged(bool);
	void periodicCollectionActiveChanged(bool);
	void snapshotIntervalInMinutesChanged(int);
	void takingSnapshotChanged(bool);
	void nextCollectionTimestampChanged(QString);

	void shutdown();

private:
	void setCollectionFailed(bool);
	void setTakingSnapshot(bool);

private slots:
	void snapshotterFinished(int, QProcess::ExitStatus);
	void snapshotterError(QProcess::ProcessError);
	void snapshotterReadyRead();
	void periodicCollectionTimerTimeout();

private:
	bool _collectionFailed : 1;
	bool _periodicCollectionActive : 1;
	bool _takingSnapshot : 1;
	int _snapshotIntervalInMinutes;
	QTimer _periodicCollectionTimer;
	QProcess _snapshotter;
	QDateTime _nextTimerTimeout;
	Settings _settings;
	SnapshotterLog _snapshotterLog;
};

#endif /* ENDURANCEDAEMON_H */
