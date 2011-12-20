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

#ifndef ENDURANCEREPORTINGBASE_H
#define ENDURANCEREPORTINGBASE_H

#include <QObject>
#include <QProcess>

class EnduranceDirectoryModel;

class EnduranceReportingBase : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool available
			READ available
			NOTIFY availableChanged)
	Q_PROPERTY(bool generationInProgress
			READ generationInProgress
			NOTIFY generationInProgressChanged)
	Q_PROPERTY(bool generationError
			READ generationError
			NOTIFY generationErrorChanged)
	Q_PROPERTY(QString log
			READ log
			NOTIFY logChanged)
	Q_PROPERTY(EnduranceDirectoryModel *enduranceDirectoryModel
			READ enduranceDirectoryModel
			WRITE setEnduranceDirectoryModel
			NOTIFY enduranceDirectoryModelChanged)

public:
	EnduranceReportingBase(QObject *parent = 0);

	bool available() const { return _available; }
	bool generationInProgress() const { return _generationInProgress; }
	bool generationError() const { return _generationError; }
	bool logAvailable() const { return !_log.isEmpty(); }
	QString log() const { return _log; }
	EnduranceDirectoryModel *enduranceDirectoryModel() { return _enduranceDirectoryModel; }
	void setEnduranceDirectoryModel(EnduranceDirectoryModel *);

public slots:
	void clearLog();
	virtual void generate(int first, int next) = 0;
	virtual void open() = 0;

signals:
	void availableChanged();
	void generationInProgressChanged();
	void generationErrorChanged();
	void logChanged();
	void enduranceDirectoryModelChanged();

protected slots:
	virtual void runnerFinished(int, QProcess::ExitStatus) = 0;
	void runnerError(QProcess::ProcessError);
	void runnerReadyRead();

protected:
	void appendLog(const QByteArray &);
	void setGenerationError(bool);

protected:
	bool _generationInProgress : 1;
	bool _generationError : 1;
	bool _available : 1;
	QProcess _runner;
	QProcess _opener;
	QByteArray _log;
	EnduranceDirectoryModel *_enduranceDirectoryModel;
};

#endif /* ENDURANCEREPORTINGBASE_H */
