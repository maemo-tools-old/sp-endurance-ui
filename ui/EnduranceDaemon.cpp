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

EnduranceDaemon::EnduranceDaemon(QObject *parent)
	: QObject(parent)
	, _collectionFailed(false)
	, _periodicCollectionActive(false)
	, _takingSnapshot(false)
	, _valid(false)
	, _snapshotIntervalInMinutes(1)
	, _enduranceDaemon(QLatin1String("com.nokia.EnduranceDaemon"),
		QLatin1String("/com/nokia/EnduranceDaemon"), QDBusConnection::systemBus(), parent)
	, _serviceWatcher(0)
{
	connect(&_enduranceDaemon, SIGNAL(collectionFailedChanged(bool)),
			this, SLOT(collectionFailedChangedSlot(bool)));
	connect(&_enduranceDaemon, SIGNAL(periodicCollectionActiveChanged(bool)),
			this, SLOT(periodicCollectionActiveChangedSlot(bool)));
	connect(&_enduranceDaemon, SIGNAL(snapshotIntervalInMinutesChanged(int)),
			this, SLOT(snapshotIntervalInMinutesChangedSlot(int)));
	connect(&_enduranceDaemon, SIGNAL(takingSnapshotChanged(bool)),
			this, SLOT(takingSnapshotChangedSlot(bool)));
	connect(&_enduranceDaemon, SIGNAL(nextCollectionTimestampChanged(const QString&)),
			this, SLOT(nextCollectionTimestampChangedSlot(const QString&)));
	_collectionFailed = _enduranceDaemon.collectionFailed();
	_periodicCollectionActive = _enduranceDaemon.periodicCollectionActive();
	_takingSnapshot = _enduranceDaemon.takingSnapshot();
	_snapshotIntervalInMinutes = _enduranceDaemon.snapshotIntervalInMinutes();
	_nextCollectionTimestamp = _enduranceDaemon.nextCollectionTimestamp();

	 _valid = _enduranceDaemon.isValid();

	 _serviceWatcher = new QDBusServiceWatcher("com.nokia.EnduranceDaemon", QDBusConnection::systemBus(),
			 QDBusServiceWatcher::WatchForOwnerChange, this);
	 connect(_serviceWatcher, SIGNAL(serviceUnregistered(const QString &)),
			 this, SLOT(serviceUnregistered(const QString&)));
	 connect(_serviceWatcher, SIGNAL(serviceRegistered(const QString &)),
			 this, SLOT(serviceRegistered(const QString&)));
}

void EnduranceDaemon::collectionFailedChangedSlot(bool newValue)
{
	if (_collectionFailed == newValue)
		return;
	_collectionFailed = newValue;
	emit collectionFailedChanged();
}

void EnduranceDaemon::periodicCollectionActiveChangedSlot(bool newValue)
{
	if (_periodicCollectionActive == newValue)
		return;
	_periodicCollectionActive = newValue;
	emit periodicCollectionActiveChanged();
}

void EnduranceDaemon::snapshotIntervalInMinutesChangedSlot(int newValue)
{
	if (_snapshotIntervalInMinutes == newValue)
		return;
	_snapshotIntervalInMinutes = newValue;
	emit snapshotIntervalInMinutesChanged();
}

void EnduranceDaemon::takingSnapshotChangedSlot(bool newValue)
{
	if (_takingSnapshot == newValue)
		return;
	_takingSnapshot = newValue;
	emit takingSnapshotChanged();
}

void EnduranceDaemon::nextCollectionTimestampChangedSlot(const QString &newValue)
{
	if (_nextCollectionTimestamp == newValue)
		return;
	_nextCollectionTimestamp = newValue;
	emit nextCollectionTimestampChanged();
}

bool EnduranceDaemon::collectionFailed() const
{
	return _collectionFailed;
}

bool EnduranceDaemon::periodicCollectionActive() const
{
	return _periodicCollectionActive;
}

void EnduranceDaemon::setPeriodicCollectionActive(bool value)
{
	_enduranceDaemon.setPeriodicCollectionActive(value);
}

int EnduranceDaemon::snapshotIntervalInMinutes() const
{
	return _snapshotIntervalInMinutes;
}

void EnduranceDaemon::setSnapshotIntervalInMinutes(int value)
{
	_enduranceDaemon.setSnapshotIntervalInMinutes(value);
}

QString EnduranceDaemon::nextCollectionTimestamp() const
{
	return _nextCollectionTimestamp;
}

bool EnduranceDaemon::takingSnapshot() const
{
	return _takingSnapshot;
}

void EnduranceDaemon::takeSnapshot()
{
	_enduranceDaemon.takeSnapshot();
}

bool EnduranceDaemon::tryShutdown()
{
	return  _enduranceDaemon.tryShutdown();
}

bool EnduranceDaemon::valid()
{
	return _valid;
}


void EnduranceDaemon::serviceRegistered(const QString &name)
{
	Q_UNUSED(name);
	if (!_valid) {
		_valid = true;
		emit validChanged();
	}
}

void EnduranceDaemon::serviceUnregistered(const QString &name)
{
	Q_UNUSED(name);
	emit terminated();
}
