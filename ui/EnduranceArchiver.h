#ifndef ENDURANCECOMPRESSOR_H
#define ENDURANCECOMPRESSOR_H

#include <QObject>
#include <QProcess>
#include <QByteArray>

class EnduranceDirectoryModel;

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
	Q_PROPERTY(EnduranceDirectoryModel *enduranceDirectoryModel
			READ enduranceDirectoryModel
			WRITE setEnduranceDirectoryModel
			NOTIFY enduranceDirectoryModelChanged)

public:
	EnduranceArchiver(QObject *parent = NULL);
	EnduranceDirectoryModel *enduranceDirectoryModel() { return _enduranceDirectoryModel; }
	void setEnduranceDirectoryModel(EnduranceDirectoryModel *);
	bool archiveInProgress() const { return _archiveInProgress; }
	bool archiveError() const { return _archiveError; }
	const QString &log() const { return _log; }

public slots:
	void archive();
	void clearLog();
	void abort();

signals:
	void archiveInProgressChanged();
	void archiveErrorChanged();
	void logChanged();
	void enduranceDirectoryModelChanged();

private slots:
	void runnerFinished(int, QProcess::ExitStatus);
	void runnerError(QProcess::ProcessError);
	void runnerReadyRead();

private:
	void appendLog(const QByteArray &);
	void setArchiveError(bool);

private:
	bool _archiveInProgress : 1;
	bool _archiveError : 1;
	QProcess _runner;
	QString _log;
	EnduranceDirectoryModel *_enduranceDirectoryModel;
};

#endif /* ENDURANCECOMPRESSOR_H */
