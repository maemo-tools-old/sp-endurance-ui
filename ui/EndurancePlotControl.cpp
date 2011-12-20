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
#include "EnduranceDirectoryModel.h"
#include "EndurancePlotControl.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>

static bool
checkEndurancePlot()
{
	return QFile::exists(ENDURANCE_PLOT_HTML);
}

EndurancePlotControl::EndurancePlotControl(QObject *parent)
	: EnduranceReportingBase(parent)
{
	_runner.setWorkingDirectory(TMPDIR);
	if (checkEndurancePlot()) {
		_available = true;
		emit availableChanged();
	}
}

void EndurancePlotControl::generate(int first, int next)
{
	if (!_enduranceDirectoryModel)
		return;
	QStringList dirList = _enduranceDirectoryModel->directoryList();
	if (dirList.isEmpty() || dirList.count() < 2)
		return;
	_generationInProgress = true;
	emit generationInProgressChanged();
	setGenerationError(false);
	clearLog();
	appendLog(tr("<font color=\"green\">Started at %1.</font><br/>")
			.arg(QDateTime::currentDateTime().toString()).toAscii());
	QStringList opts;
	opts << "/usr/bin/endurance_plot";
	for (int i = first - 1; i < dirList.size() - 1; i += next) {
		opts << (DATADIR ".endurance/" + dirList[i]);
	}
	opts << (DATADIR ".endurance/" + dirList[dirList.size() - 1]);
	_runner.start("/usr/bin/nice", opts, QIODevice::ReadOnly);
}

static void
nukeDir(const QString &dirname)
{
	QDir dir(dirname);
	QDirIterator it(dir);
	while (it.hasNext()) {
		QString entry = it.next();
		QFile::remove(entry);
	}
	QDir().rmdir(dirname);
}

void EndurancePlotControl::runnerFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << Q_FUNC_INFO << ": exitCode =" << exitCode << ", exitStatus =" << exitStatus;
	_generationInProgress = false;
	emit generationInProgressChanged();
	QString logMsg;
	QString timeStamp = QDateTime::currentDateTime().toString();
	if (exitCode) {
		logMsg = tr("<font color=\"red\">Finished with failure at %1.</font>\n")
			.arg(timeStamp);
		goto error;
	}
	QDir().mkpath(REPORTDIR);
	nukeDir(TMPDIR "e");
	nukeDir(REPORTDIR "graphs");
	QFile::remove(ENDURANCE_PLOT_HTML);
	if (!QFile::copy(ENDURANCE_PLOT_HTML_TMP, ENDURANCE_PLOT_HTML)) {
		logMsg = tr("<font color=\"red\">Failed to copy '%1' to '%2'.</font>\n")
			.arg(ENDURANCE_PLOT_HTML_TMP).arg(ENDURANCE_PLOT_HTML);
		goto error;
	}
	{
		// Copy the 'graphs' directory.
		QDir().mkdir(REPORTDIR "graphs");
		QDirIterator it(QDir(TMPDIR "graphs"));
		while (it.hasNext()) {
			QString entry = it.next();
			if (it.fileName() == "." || it.fileName() == "..")
				continue;
			QString src = TMPDIR "graphs/" + it.fileName();
			QString dst = REPORTDIR "graphs/" + it.fileName();
			QFile::copy(src, dst);
			QFile::remove(entry);
		}
		QDir().rmdir(TMPDIR "graphs");
	}
	logMsg = tr("<font color=\"green\">Finished at %1.</font>\n").arg(timeStamp);
	goto success;
error:
	setGenerationError(true);
success:
	QFile::remove(ENDURANCE_PLOT_HTML_TMP);
	appendLog(logMsg.toAscii());
	if (checkEndurancePlot()) {
		_available = true;
		emit availableChanged();
	}
}

void EndurancePlotControl::open()
{
	_opener.start("xdg-open", QStringList() << ENDURANCE_PLOT_HTML);
}
