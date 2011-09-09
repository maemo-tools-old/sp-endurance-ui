#ifndef ENDURANCEREPORTINGBASE_H
#define ENDURANCEREPORTINGBASE_H

#include <QObject>
#include <QProcess>

class EnduranceDirectoryModel;

class EnduranceReportingBase : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool available
			READ available
			NOTIFY availableChanged)
	Q_PROPERTY(bool generationInProgress
			READ generationInProgress
			NOTIFY generationInProgressChanged)
	Q_PROPERTY(bool generationError
			READ generationError
			NOTIFY generationErrorChanged)
	Q_PROPERTY(QString log
			READ log
			NOTIFY logChanged)
	Q_PROPERTY(EnduranceDirectoryModel *enduranceDirectoryModel
			READ enduranceDirectoryModel
			WRITE setEnduranceDirectoryModel
			NOTIFY enduranceDirectoryModelChanged)

public:
	EnduranceReportingBase(QObject *parent = 0);

	bool available() const { return _available; }
	bool generationInProgress() const { return _generationInProgress; }
	bool generationError() const { return _generationError; }
	bool logAvailable() const { return !_log.isEmpty(); }
	QString log() const { return _log; }
	EnduranceDirectoryModel *enduranceDirectoryModel() { return _enduranceDirectoryModel; }
	void setEnduranceDirectoryModel(EnduranceDirectoryModel *);

public slots:
	void clearLog();
	virtual void generate() = 0;
	virtual void open() = 0;

signals:
	void availableChanged();
	void generationInProgressChanged();
	void generationErrorChanged();
	void logChanged();
	void enduranceDirectoryModelChanged();

protected slots:
	virtual void runnerFinished(int, QProcess::ExitStatus) = 0;
	void runnerError(QProcess::ProcessError);
	void runnerReadyRead();

protected:
	void appendLog(const QByteArray &);
	void setGenerationError(bool);

protected:
	bool _generationInProgress : 1;
	bool _generationError : 1;
	bool _available : 1;
	QProcess _runner;
	QProcess _opener;
	QByteArray _log;
	EnduranceDirectoryModel *_enduranceDirectoryModel;
};

#endif /* ENDURANCEREPORTINGBASE_H */
