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
#ifndef ENDURANCECOMPRESSOR_H
#define ENDURANCECOMPRESSOR_H

#include <QObject>
#include <QProcess>
#include <QByteArray>

class ArchiverOptions;

class EnduranceArchiver : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool archiveInProgress
			READ archiveInProgress
			NOTIFY archiveInProgressChanged)
	Q_PROPERTY(bool archiveError
			READ archiveError
			NOTIFY archiveErrorChanged)
	Q_PROPERTY(QString log
			READ log
			NOTIFY logChanged)
	Q_PROPERTY(QString outputFilename
			READ outputFilename
			NOTIFY outputFilenameChanged)

public:
	EnduranceArchiver(QObject *parent = NULL);
	bool archiveInProgress() const { return _archiveInProgress; }
	bool archiveError() const { return _archiveError; }
	const QString &log() const { return _log; }
	const QString &outputFilename() const { return _outputFilename; }

public slots:
	void archive(ArchiverOptions *);
	void clearLog();
	void abort();

signals:
	void archiveInProgressChanged();
	void archiveErrorChanged();
	void logChanged();
	void outputFilenameChanged();

private slots:
	void runnerFinished(int, QProcess::ExitStatus);
	void runnerError(QProcess::ProcessError);
	void runnerReadyRead();

private:
	void appendLog(const QByteArray &);
	void setArchiveError(bool);
	QString createArchiveName(const QString& prefix);

private:
	bool _archiveInProgress : 1;
	bool _archiveError : 1;
	QProcess _runner;
	QString _log;
	QString _outputFilename;
};

#endif /* ENDURANCECOMPRESSOR_H */
