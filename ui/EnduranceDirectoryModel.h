#ifndef ENDURANCEDIRECTORYMODEL_H
#define ENDURANCEDIRECTORYMODEL_H

#include <QObject>
#include <QFileSystemModel>
#include <QFutureWatcher>
#include <QHash>

struct DirectoryInfo
{
	DirectoryInfo() : dirsize(0) {}
	QString dirname;
	QString date;
	quint64 dirsize;
};

class EnduranceDirectoryModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(bool clearing READ clearing NOTIFY clearingChanged)
	Q_PROPERTY(int totalSizeMB READ totalSizeMB NOTIFY totalSizeMBChanged)

public:
	EnduranceDirectoryModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	bool clearing() const { return _clearInProgress; }
	QStringList directoryList() const;
	int totalSizeMB() const;

public slots:
	void clearEnduranceData();
	//bool rmdir(const QModelIndex &index) const;

signals:
	void clearingChanged();
	void totalSizeMBChanged();

private slots:
	void slotRowsInserted(const QModelIndex &parent, int begin, int end);
	void slotRowsRemoved(const QModelIndex &parent, int begin, int end);
	void slotRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
	void slotDirInfoResultsReadyAt(int beginIndex, int endIndex);
	void clearFinished();

private:
	QModelIndex fsRootIndex() const;
	void kickDirInfoWatcher();

private:
	bool _clearInProgress;
	bool _dirInfoWatcherBusy;
	QFutureWatcher<DirectoryInfo> _dirInfoWatcher;
	QList<QString> _dirInfoQueue;
	QFutureWatcher<void> _clearWatcher;
	QHash<QString, DirectoryInfo> _directories;
	QFileSystemModel _fsModel;
};

#endif /* ENDURANCEDIRECTORYMODEL_H */
