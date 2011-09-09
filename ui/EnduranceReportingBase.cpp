#include "EnduranceDirectoryModel.h"
#include "EnduranceReportingBase.h"
#include <QDateTime>
#include <QDebug>

EnduranceReportingBase::EnduranceReportingBase(QObject *parent)
	: QObject(parent)
	, _generationInProgress(false)
	, _generationError(false)
	, _available(false)
	, _enduranceDirectoryModel(NULL)
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

void EnduranceReportingBase::runnerError(QProcess::ProcessError error)
{
	qDebug() << Q_FUNC_INFO << ": " << error;
	_generationInProgress = false;
	emit generationInProgressChanged();
	setGenerationError(true);
	appendLog(tr("<font color=\"red\">Finished with failure at %1.</font>\n").arg(QDateTime::currentDateTime().toString()).toAscii());
}

void EnduranceReportingBase::runnerReadyRead()
{
	qDebug() << Q_FUNC_INFO;
	appendLog(_runner.readAllStandardOutput());
	appendLog(_runner.readAllStandardError());
}

void EnduranceReportingBase::appendLog(const QByteArray &str)
{
	if (str.isEmpty())
		return;
	qDebug() << Q_FUNC_INFO << ":" << str;
	_log += QByteArray(str).replace('\n', "<br/>");
	emit logChanged();
}

void EnduranceReportingBase::clearLog()
{
	qDebug() << Q_FUNC_INFO;
	_log.clear();
	emit logChanged();
	setGenerationError(false);
}

void EnduranceReportingBase::setEnduranceDirectoryModel(EnduranceDirectoryModel *model)
{
	_enduranceDirectoryModel = model;
	emit enduranceDirectoryModelChanged();
}

void EnduranceReportingBase::setGenerationError(bool newValue)
{
	if (_generationError == newValue)
		return;
	_generationError = newValue;
	emit generationErrorChanged();
}
