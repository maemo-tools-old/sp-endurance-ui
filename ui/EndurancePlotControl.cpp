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

void EndurancePlotControl::generate()
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
	foreach(const QString &dir, dirList)
		opts << (DATADIR ".endurance/" + dir);
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
