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
#include "EnduranceDaemon.h"
#include <QDebug>
#include <syslog.h>

#define SNAPSHOT_PATH "/home/user/MyDocs/.sp-endurance/.endurance"
static const QStringList SNAPSHOT_ARGS(SNAPSHOT_PATH);

#define MINIMUM_INTERVAL_MINUTES 1
#define DEFAULT_INTERVAL_MINUTES (4*60)

EnduranceDaemon::EnduranceDaemon(QObject *parent)
	: QObject(parent)
	, _collectionFailed(false)
	, _periodicCollectionActive(false)
	, _takingSnapshot(false)
	, _snapshotIntervalInMinutes(DEFAULT_INTERVAL_MINUTES)
{
	openlog("com.nokia.EnduranceDaemon", LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, "Daemon starting.");
	setSnapshotIntervalInMinutes(_settings.snapshotIntervalInMinutes());
	setPeriodicCollectionActive(_settings.periodicCollectionActive());
	connect(&_snapshotter, SIGNAL(finished(int, QProcess::ExitStatus)),
			SLOT(snapshotterFinished(int, QProcess::ExitStatus)));
	connect(&_snapshotter, SIGNAL(error(QProcess::ProcessError)),
			SLOT(snapshotterError(QProcess::ProcessError)));
	connect(&_snapshotter, SIGNAL(readyReadStandardOutput()),
			SLOT(snapshotterReadyRead()));
	connect(&_snapshotter, SIGNAL(readyReadStandardError()),
			SLOT(snapshotterReadyRead()));
	connect(&_periodicCollectionTimer, SIGNAL(timeout()),
			this, SLOT(periodicCollectionTimerTimeout()));
}

bool EnduranceDaemon::periodicCollectionActive() const
{
	qDebug() << Q_FUNC_INFO;
	return _periodicCollectionActive;
}

void EnduranceDaemon::setPeriodicCollectionActive(bool newValue)
{
	qDebug() << Q_FUNC_INFO << newValue;
	if (newValue == _periodicCollectionActive)
		return;
	_settings.setPeriodicCollectionActive(newValue);
	if (newValue) {
		_periodicCollectionTimer.setInterval(_snapshotIntervalInMinutes*60*1000);
		if (_periodicCollectionTimer.interval() < MINIMUM_INTERVAL_MINUTES*60*1000)
			return;
		syslog(LOG_INFO, "Starting periodic endurance data collection "
				"with %d minute interval.",
				_snapshotIntervalInMinutes);
		_periodicCollectionTimer.start();
		_nextTimerTimeout = QDateTime::currentDateTime().addMSecs(_periodicCollectionTimer.interval());
	} else {
		_periodicCollectionTimer.stop();
	}
	_periodicCollectionActive = newValue;
	emit periodicCollectionActiveChanged(_periodicCollectionActive);
	if (newValue)
		takeSnapshot();
	emit nextCollectionTimestampChanged(nextCollectionTimestamp());
}

void EnduranceDaemon::periodicCollectionTimerTimeout()
{
	qDebug() << Q_FUNC_INFO;
	_nextTimerTimeout = QDateTime::currentDateTime().addMSecs(_periodicCollectionTimer.interval());
	emit nextCollectionTimestampChanged(nextCollectionTimestamp());
	takeSnapshot();
}

int EnduranceDaemon::snapshotIntervalInMinutes() const
{
	qDebug() << Q_FUNC_INFO;
	return _snapshotIntervalInMinutes;
}

void EnduranceDaemon::setSnapshotIntervalInMinutes(int newValue)
{
	qDebug() << Q_FUNC_INFO << ": newValue:" << newValue;
	if (newValue != _snapshotIntervalInMinutes) {
		if (newValue >= MINIMUM_INTERVAL_MINUTES) {
			_snapshotIntervalInMinutes = newValue;
			emit snapshotIntervalInMinutesChanged(newValue);
			_settings.setSnapshotIntervalInMinutes(newValue);
		}
	}
}

bool EnduranceDaemon::takingSnapshot() const
{
	qDebug() << Q_FUNC_INFO;
	return _takingSnapshot;
}

void EnduranceDaemon::takeSnapshot()
{
	qDebug() << Q_FUNC_INFO;
	if (!_takingSnapshot) {
		_snapshotterLog.reopen();
		setTakingSnapshot(true);
		_snapshotter.start("/usr/bin/save-incremental-endurance-stats",
				SNAPSHOT_ARGS, QIODevice::ReadOnly);
		syslog(LOG_INFO, "Taking endurance snapshot.");
	}
}

void EnduranceDaemon::snapshotterFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << Q_FUNC_INFO << ": exitCode =" << exitCode << ", exitStatus =" << exitStatus;
	setTakingSnapshot(false);
	if (exitCode || exitStatus != QProcess::NormalExit) {
		setCollectionFailed(true);
	} else {
		setCollectionFailed(false);
	}
}

void EnduranceDaemon::snapshotterError(QProcess::ProcessError processError)
{
	qDebug() << Q_FUNC_INFO << ": processError" << processError;
	setTakingSnapshot(false);
	setCollectionFailed(true);
}

void EnduranceDaemon::setCollectionFailed(bool newValue)
{
	if (newValue != _collectionFailed) {
		_collectionFailed = newValue;
		emit collectionFailedChanged(_collectionFailed);
	}
}

void EnduranceDaemon::setTakingSnapshot(bool newValue)
{
	qDebug() << Q_FUNC_INFO << ":" << newValue;
	if (newValue != _takingSnapshot) {
		_takingSnapshot = newValue;
		emit takingSnapshotChanged(_takingSnapshot);
	}
}

bool EnduranceDaemon::collectionFailed() const
{
	return _collectionFailed;
}

QString EnduranceDaemon::nextCollectionTimestamp() const
{
	if (!_periodicCollectionActive)
		return QString();
	return _nextTimerTimeout.toString();
}

void EnduranceDaemon::snapshotterReadyRead()
{
	_snapshotterLog.log(_snapshotter.readAllStandardOutput());
	_snapshotterLog.log(_snapshotter.readAllStandardError());
}

bool EnduranceDaemon::tryShutdown()
{
	qDebug() << "Attempting to shutdown endurance daemon:" << _periodicCollectionActive;
	if (_periodicCollectionActive) {
		/* prevent from terminated if periodic snapshot collection is active */
		return false;
	}
	emit shutdown();
	return true;
}
