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

#include "EnduranceLogReader.h"
#include "EnduranceConstants.h"

#include <QFile>
#include <QDebug>

EnduranceLogReader::EnduranceLogReader(QObject* parent)
	: QObject(parent)
{
	connect(&_watcher, SIGNAL(fileChanged(const QString&)), SIGNAL(logChanged()));
}

QString EnduranceLogReader::log() const
{
	QFile logfile(ENDURANCE_LOG_FILE);
	if (!logfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return "";
	}
	QString contents;
	while (!logfile.atEnd()) {
		contents += logfile.readLine() + "<br/>";
	}
	return contents;
}

void EnduranceLogReader::startMonitoring()
{
	emit logChanged();
	_watcher.addPath(ENDURANCE_LOG_FILE);
}

void EnduranceLogReader::stopMonitoring()
{
	_watcher.removePath(ENDURANCE_LOG_FILE);
}
