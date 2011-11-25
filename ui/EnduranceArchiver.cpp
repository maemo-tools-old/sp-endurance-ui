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
#include "EnduranceConstants.h"
#include "EnduranceArchiver.h"
#include "EnduranceDirectoryModel.h"
#include "ArchiverOptions.h"

#include <sys/types.h>
#include <signal.h>
#include <QDateTime>

EnduranceArchiver::EnduranceArchiver(QObject *parent)
	: QObject(parent)
	, _archiveInProgress(false)
	, _archiveError(false)
{
	connect(&_runner, SIGNAL(finished(int, QProcess::ExitStatus)),
			SLOT(runnerFinished(int, QProcess::ExitStatus)));
	connect(&_runner, SIGNAL(error(QProcess::ProcessError)),
			SLOT(runnerError(QProcess::ProcessError)));
	connect(&_runner, SIGNAL(readyReadStandardOutput()),
			SLOT(runnerReadyRead()));
	connect(&_runner, SIGNAL(readyReadStandardError()),
			SLOT(runnerReadyRead()));
}

void EnduranceArchiver::archive(ArchiverOptions *options)
{
	if (!options)
		return;
	QStringList dirList = options->contents();
	if (dirList.isEmpty()) {
		setArchiveError(true);
		appendLog(tr("<font color=\"red\">No source data found.</font>\n").toUtf8());
		return;
	}
	setArchiveError(false);
	_archiveInProgress = true;
	emit archiveInProgressChanged();
	appendLog(tr("<font color=\"green\">Started.</font><br/>").toUtf8());
	QStringList opts;
	opts << "/usr/bin/zip";
	opts << "-r";
	_outputFilename = createArchiveName(options->outputTemplate());
	emit outputFilenameChanged();
	opts << options->outputPath() + _outputFilename;
	foreach(const QString &dir, dirList)
		opts << (options->dataDir() + "/" + dir);

	_runner.setWorkingDirectory(options->workDir());
	_runner.start("/usr/bin/nice", opts, QIODevice::ReadOnly);
}

void EnduranceArchiver::runnerFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << Q_FUNC_INFO << ": exitCode =" << exitCode << ", exitStatus =" << exitStatus;
	_archiveInProgress = false;
	emit archiveInProgressChanged();
	QString logMsg;
	if (exitCode) {
		logMsg = tr("<font color=\"red\">Failure.</font>\n");
		setArchiveError(true);
	} else {
		logMsg = tr("<font color=\"green\">OK.</font>\n");
	}
	appendLog(logMsg.toAscii());
}

void EnduranceArchiver::runnerError(QProcess::ProcessError error)
{
	qDebug() << Q_FUNC_INFO << ": " << error;
	_archiveInProgress = false;
	emit archiveInProgressChanged();
	setArchiveError(true);
	appendLog(tr("<font color=\"red\">Failure.</font>\n").toUtf8());
}

void EnduranceArchiver::runnerReadyRead()
{
	qDebug() << Q_FUNC_INFO;
	appendLog(_runner.readAllStandardOutput());
	appendLog(_runner.readAllStandardError());
}

void EnduranceArchiver::appendLog(const QByteArray &str)
{
	if (str.isEmpty())
		return;
	qDebug() << Q_FUNC_INFO << ":" << str;
	_log += QByteArray(str).replace('\n', "<br/>");
	emit logChanged();
}

void EnduranceArchiver::clearLog()
{
	qDebug() << Q_FUNC_INFO;
	_log.clear();
	emit logChanged();
}

void EnduranceArchiver::abort()
{
	if (!_archiveInProgress)
		return;
	if (_runner.pid()) {
		/* /usr/bin/zip exits with cleanup when is gets SIGINT. */
		kill(_runner.pid(), SIGINT);
	}
}

void EnduranceArchiver::setArchiveError(bool newValue)
{
	if (newValue == _archiveError)
		return;
	_archiveError = newValue;
	emit archiveErrorChanged();
}


QString EnduranceArchiver::createArchiveName(const QString &prefix)
{
	QString name(prefix);
	name += QDateTime::currentDateTime().toString("_yyyy.MM.dd-hh.mm");
	name += ".zip";
	return name;
}

