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

#ifndef ENDURANCELOGREADER_H
#define ENDURANCELOGREADER_H

#include <QObject>
#include <QFileSystemWatcher>

/**
 * The EnduranceLogReader provides access to endurance daemon
 * snapshot logs for QML elements.
 *
 * 1) to access the log contents use the EnduranceLogReader::log property
 * 2) before displaying element using the log property call
 *    EnduranceLogReader::startMonitoring() method to keep the property up
 *    to date.
 * 3) When this element becomes inactive, disable the log property update
 *    with EnduranceLogReader::stopMonitoring()
 *
 * Because endurance daemon resets the log file before snapshot, EnduranceLogReader
 * always reads the whole log file instead of reading only added lines. Not the
 * most effective way, but good enough in this case.
 */
class EnduranceLogReader : public QObject {
	Q_OBJECT

	Q_PROPERTY(QString log
			READ log
			NOTIFY logChanged)

public:
	EnduranceLogReader(QObject *parent = 0);
	QString log() const;

public slots:
	void startMonitoring();
	void stopMonitoring();

signals:
	void logChanged();

private:
	QFileSystemWatcher _watcher;
};

#endif /* ENDURANCELOGREADER_H */
