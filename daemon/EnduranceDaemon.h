#ifndef ENDURANCEDAEMON_H
#define ENDURANCEDAEMON_H

#include "Settings.h"
#include "SnapshotterLog.h"
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QDateTime>

class EnduranceDaemon: public QObject
{
	Q_OBJECT
public:
	EnduranceDaemon(QObject *parent = 0);
	~EnduranceDaemon() {}

public:
	Q_PROPERTY(bool collectionFailed READ collectionFailed)
	bool collectionFailed() const;

	Q_PROPERTY(bool periodicCollectionActive READ periodicCollectionActive WRITE setPeriodicCollectionActive)
	bool periodicCollectionActive() const;
	void setPeriodicCollectionActive(bool value);

	Q_PROPERTY(int snapshotIntervalInMinutes READ snapshotIntervalInMinutes WRITE setSnapshotIntervalInMinutes)
	int snapshotIntervalInMinutes() const;
	void setSnapshotIntervalInMinutes(int value);

	Q_PROPERTY(bool takingSnapshot READ takingSnapshot)
	bool takingSnapshot() const;

	Q_PROPERTY(QString nextCollectionTimestamp READ nextCollectionTimestamp NOTIFY nextCollectionTimestampChanged)
	QString nextCollectionTimestamp() const;

public slots:
	void takeSnapshot();

signals:
	void collectionFailedChanged(bool);
	void periodicCollectionActiveChanged(bool);
	void snapshotIntervalInMinutesChanged(int);
	void takingSnapshotChanged(bool);
	void nextCollectionTimestampChanged(QString);

private:
	void setCollectionFailed(bool);
	void setTakingSnapshot(bool);

private slots:
	void snapshotterFinished(int, QProcess::ExitStatus);
	void snapshotterError(QProcess::ProcessError);
	void snapshotterReadyRead();
	void periodicCollectionTimerTimeout();

private:
	bool _collectionFailed : 1;
	bool _periodicCollectionActive : 1;
	bool _takingSnapshot : 1;
	int _snapshotIntervalInMinutes;
	QTimer _periodicCollectionTimer;
	QProcess _snapshotter;
	QDateTime _nextTimerTimeout;
	Settings _settings;
	SnapshotterLog _snapshotterLog;
};

#endif /* ENDURANCEDAEMON_H */
