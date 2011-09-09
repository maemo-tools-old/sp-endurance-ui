#include "EnduranceConstants.h"
#include "EnduranceArchiver.h"
#include "EnduranceDirectoryModel.h"
#include <sys/types.h>
#include <signal.h>

EnduranceArchiver::EnduranceArchiver(QObject *parent)
	: QObject(parent)
	, _archiveInProgress(false)
	, _archiveError(false)
	, _enduranceDirectoryModel(NULL)
{
	_runner.setWorkingDirectory(DATADIR);
	connect(&_runner, SIGNAL(finished(int, QProcess::ExitStatus)),
			SLOT(runnerFinished(int, QProcess::ExitStatus)));
	connect(&_runner, SIGNAL(error(QProcess::ProcessError)),
			SLOT(runnerError(QProcess::ProcessError)));
	connect(&_runner, SIGNAL(readyReadStandardOutput()),
			SLOT(runnerReadyRead()));
	connect(&_runner, SIGNAL(readyReadStandardError()),
			SLOT(runnerReadyRead()));
}

void EnduranceArchiver::archive()
{
	if (!_enduranceDirectoryModel)
		return;
	QStringList dirList = _enduranceDirectoryModel->directoryList();
	if (dirList.isEmpty()) {
		setArchiveError(true);
		appendLog(tr("<font color=\"red\">No snapshots collected.</font>\n").toUtf8());
		return;
	}
	setArchiveError(false);
	QFile::remove("/home/user/MyDocs/Documents/sp-endurance.zip");
	_archiveInProgress = true;
	emit archiveInProgressChanged();
	appendLog(tr("<font color=\"green\">Started.</font><br/>").toUtf8());
	QStringList opts;
	opts << "/usr/bin/zip";
	opts << "-r";
	opts << "/home/user/MyDocs/Documents/sp-endurance.zip";
	foreach(const QString &dir, dirList)
		opts << (".endurance/" + dir);
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

void EnduranceArchiver::setEnduranceDirectoryModel(EnduranceDirectoryModel *model)
{
	_enduranceDirectoryModel = model;
	emit enduranceDirectoryModelChanged();
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
