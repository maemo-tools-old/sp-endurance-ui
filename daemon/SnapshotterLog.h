#ifndef SNAPSHOTTERLOG_H
#define SNAPSHOTTERLOG_H

#include <QFile>
#include <QTextStream>

class SnapshotterLog
{
public:
	SnapshotterLog();
	void reopen();
	void log(const QByteArray &message);
private:
	QFile _logFile;
	QTextStream _logStream;
};

#endif /* SNAPSHOTTERLOG_H */
