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

#include "SnapshotterLog.h"
#include <QDir>

static const QLatin1String SNAPSHOTTERLOG_PATH("/var/log/enduranced");
static const QLatin1String SNAPSHOTTERLOG_FILE("/var/log/enduranced/snapshotter");

SnapshotterLog::SnapshotterLog()
	: _logFile(SNAPSHOTTERLOG_FILE)
{
	QDir().mkpath(SNAPSHOTTERLOG_PATH);
}

void SnapshotterLog::reopen()
{
	_logStream.reset();
	_logStream.setDevice(NULL);
	_logFile.close();
	_logFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	_logStream.setDevice(&_logFile);
}

void SnapshotterLog::log(const QByteArray &message)
{
	_logStream << message;
	_logStream.flush();
}
