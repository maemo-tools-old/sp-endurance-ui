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
#include "EnduranceReportControl.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>

static bool
checkEnduranceReport()
{
	return QFile::exists(ENDURANCE_REPORT);
}

EnduranceReportControl::EnduranceReportControl(QObject *parent)
	: EnduranceReportingBase(parent)
{
	if (checkEnduranceReport()) {
		_available = true;
		emit availableChanged();
	}
}

void EnduranceReportControl::generate()
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
	_runner.setStandardOutputFile(ENDURANCE_REPORT_TMP);
	QStringList opts;
	opts << "/usr/bin/endurance_report.py";
	foreach(const QString &dir, dirList)
		opts << (DATADIR ".endurance/" + dir);
	_runner.start("/usr/bin/nice", opts, QIODevice::ReadOnly);
}

void EnduranceReportControl::runnerFinished(int exitCode, QProcess::ExitStatus exitStatus)
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
	QFile::remove(ENDURANCE_REPORT);
	if (!QFile::rename(ENDURANCE_REPORT_TMP, ENDURANCE_REPORT)) {
		logMsg = tr("<font color=\"red\">Failed to rename '%1' to '%2'.</font>\n")
			.arg(ENDURANCE_REPORT_TMP).arg(ENDURANCE_REPORT);
		goto error;
	}
	logMsg = tr("<font color=\"green\">Finished at %1.</font>\n").arg(timeStamp);
	goto success;
error:
	QFile::remove(ENDURANCE_REPORT_TMP);
	setGenerationError(true);
success:
	appendLog(logMsg.toAscii());
	if (checkEnduranceReport()) {
		_available = true;
		emit availableChanged();
	}
}

void EnduranceReportControl::open()
{
	_opener.start("xdg-open", QStringList() << ENDURANCE_REPORT);
}
