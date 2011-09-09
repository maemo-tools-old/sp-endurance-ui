#include "SnapshotterLog.h"
#include <QDir>

static const QLatin1String SNAPSHOTTERLOG_PATH("/var/log/enduranced");
static const QLatin1String SNAPSHOTTERLOG_FILE("/var/log/enduranced/snapshotter");

SnapshotterLog::SnapshotterLog()
	: _logFile(SNAPSHOTTERLOG_FILE)
{
	QDir().mkpath(SNAPSHOTTERLOG_PATH);
}

void SnapshotterLog::reopen()
{
	_logStream.reset();
	_logStream.setDevice(NULL);
	_logFile.close();
	_logFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	_logStream.setDevice(&_logFile);
}

void SnapshotterLog::log(const QByteArray &message)
{
	_logStream << message;
	_logStream.flush();
}
