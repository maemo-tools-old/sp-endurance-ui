#include "EnduranceConstants.h"
#include "EnduranceDirectoryModel.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtCore>
#include <QtConcurrentMap>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SNAPSHOTDIR (DATADIR ".endurance/")

enum EnduranceRoles {
	TitleRole = Qt::UserRole + 1,
	SubtitleRole
};

static quint64
recursiveSize(QString path)
{
	qDebug() << Q_FUNC_INFO << ":" << path;
	quint64 size = 0;
	QFileInfo info(path);
	if (info.isFile()) {
		size = info.size();
	} else if (info.isDir()) {
		QDir dir(path);
		QStringList entries = dir.entryList();
		foreach (const QString &entry, entries) {
			if (entry == "." || entry == "..")
				continue;
			size += recursiveSize(path + '/' + entry);
		}
	}
	return size;
}

#define CSV_MAX_LINES_READ 10

static QString
snapshotDirectoryDate(QString dirname)
{
	QString ret;
	QString snapshotDate;
	QFile csv(dirname + "/usage.csv");
	if (!csv.open(QIODevice::ReadOnly))
		return ret;
	QTextStream in(&csv);
	for (int i=0; i < CSV_MAX_LINES_READ && !in.atEnd(); ++i) {
		QString line = in.readLine();
		if (line.startsWith("date = ")) {
			ret = line.mid(7);
			break;
		}
	}
	return ret;
}

static DirectoryInfo
directoryInfoFor(QString dirname)
{
	qDebug() << Q_FUNC_INFO << ":" << dirname;
	DirectoryInfo dirInfo;

	/* We get notifications at the moment a new endurance directory is
	 * created, and while save-incremental-endurance-stats is still
	 * running. Check directory modified timestamps to heuristically detect
	 * when the script is done.
	 */
	for (int i=0; i < 10; ++i) {
		struct stat statbuf;
		if (stat((SNAPSHOTDIR+dirname).toUtf8(), &statbuf) == -1)
			goto ok;
		QDateTime now = QDateTime::currentDateTime();
		QDateTime modified;
		modified.setTime_t(statbuf.st_mtime);
		if (modified.secsTo(now) > 5)
			goto ok;
		sleep(1);
	}
ok:
	dirInfo.dirname = dirname;
	dirInfo.dirsize = recursiveSize(SNAPSHOTDIR+dirname);
	dirInfo.date = snapshotDirectoryDate(SNAPSHOTDIR+dirname);
	return dirInfo;
}

static void
recursiveRemove(QString path, bool removeDirectory)
{
	QDir dir(path);
	foreach(const QFileInfo &entry, dir.entryInfoList()) {
		if (entry.isFile()) {
			qDebug() << "QFile::remove(" << entry.filePath() << ");";
			QFile::remove(entry.filePath());
		} else if (entry.isDir() && entry.fileName() != "." && entry.fileName() != "..") {
			recursiveRemove(entry.filePath(), true);
			if (removeDirectory) {
				qDebug() << "QDir().rmdir(" << entry.filePath() << ");";
				QDir().rmdir(entry.filePath());
			}
		}
	}
	if (removeDirectory) {
		qDebug() << "QDir().rmdir(" << path << ");";
		QDir().rmdir(path);
	}
}

static void
doClearEnduranceData(QFutureWatcher<DirectoryInfo> *_dirInfoWatcher)
{
	_dirInfoWatcher->cancel();
	_dirInfoWatcher->waitForFinished();
	recursiveRemove(SNAPSHOTDIR, false);
}

EnduranceDirectoryModel::EnduranceDirectoryModel(QObject *parent)
	: QAbstractListModel(parent)
	, _clearInProgress(false)
	, _dirInfoWatcherBusy(false)
{
	QHash<int, QByteArray> roles;
	roles[TitleRole] = "title";
	roles[SubtitleRole] = "subtitle";
	setRoleNames(roles);

	_fsModel.setReadOnly(false);
	_fsModel.setRootPath(SNAPSHOTDIR);

	connect(&_dirInfoWatcher, SIGNAL(resultsReadyAt(int,int)),
		this, SLOT(slotDirInfoResultsReadyAt(int,int)));
	connect(&_clearWatcher, SIGNAL(finished()),
		this, SLOT(clearFinished()));

	connect(&_fsModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
		this, SLOT(slotRowsInserted(const QModelIndex &, int, int)));
	connect(&_fsModel, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)),
		this, SLOT(slotRowsAboutToBeRemoved(const QModelIndex &, int, int)));
	connect(&_fsModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
		this, SLOT(slotRowsRemoved(const QModelIndex &, int, int)));

	_fsModel.setNameFilters(QStringList()
			<< "[0-9][0-9][0-9]"
			<< "[0-9][0-9][0-9][0-9]"
			<< "[0-9][0-9][0-9][0-9][0-9]");
	_fsModel.setNameFilterDisables(false);
}

void EnduranceDirectoryModel::slotRowsInserted(const QModelIndex &parent, int start, int end)
{
	qDebug() << Q_FUNC_INFO << ":" << parent << ",start:" << start << ",end:" << end;
	Q_UNUSED(parent);
	beginInsertRows(QModelIndex(), start, end);
	for (int row=start; row <= end; ++row) {
		QModelIndex fsIndex = _fsModel.index(row, 0, fsRootIndex());
		QString path = _fsModel.data(fsIndex, Qt::DisplayRole).toString();
		if (path.isEmpty())
			continue;
		_dirInfoQueue.append(path);
	}
	kickDirInfoWatcher();
	endInsertRows();
}

void EnduranceDirectoryModel::kickDirInfoWatcher()
{
	qDebug() << Q_FUNC_INFO;
	if (_dirInfoQueue.isEmpty())
		return;
	if (_dirInfoWatcherBusy)
		return;
	_dirInfoWatcherBusy = true;
	_dirInfoWatcher.setFuture(QtConcurrent::run(directoryInfoFor,
				_dirInfoQueue.takeFirst()));
}

void EnduranceDirectoryModel::slotRowsRemoved(const QModelIndex &parent, int start, int end)
{
	qDebug() << Q_FUNC_INFO << ":" << parent << ",start:" << start << ",end:" << end;
	Q_UNUSED(parent);
	beginRemoveRows(QModelIndex(), start, end);
	endRemoveRows();
}

void EnduranceDirectoryModel::slotRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
	qDebug() << Q_FUNC_INFO << ":" << parent << ",start:" << start << ",end:" << end;
	Q_UNUSED(parent);
	if (_directories.isEmpty())
		return;
	for (int row=start; row <= end; ++row) {
		QModelIndex fsIndex = _fsModel.index(row, 0, fsRootIndex());
		QString path = _fsModel.data(fsIndex, Qt::DisplayRole).toString();
		if (path.isEmpty())
			continue;
		qDebug() << Q_FUNC_INFO << ": deleting path:" << path;
		_directories.remove(path);
	}
	emit totalSizeMBChanged();
}

int EnduranceDirectoryModel::rowCount(const QModelIndex &parent) const
{
	qDebug() << Q_FUNC_INFO << parent;
	Q_UNUSED(parent);
	int ret = _fsModel.rowCount(fsRootIndex());
	qDebug() << Q_FUNC_INFO << ":" << ret;
	return ret;
}

QVariant EnduranceDirectoryModel::data(const QModelIndex &index, int role) const
{
	qDebug() << Q_FUNC_INFO << "index:" << index << ", role:" << role;
	QModelIndex fsIndex = _fsModel.index(index.row(), 0, fsRootIndex());
	QVariant dirName = _fsModel.data(fsIndex, Qt::DisplayRole);
	if (role == TitleRole) {
		return dirName;
	}
	if (role == SubtitleRole) {
		QString path = dirName.toString();
		qDebug() << Q_FUNC_INFO << "SubtitleRole:" << path;
		if (path.isEmpty()) {
			return QVariant();
		}
		if (_directories.contains(path)) {
			qDebug() << Q_FUNC_INFO << "SubtitleRole: got dirInfo!";
			const DirectoryInfo &dirInfo = _directories[path];
			if (!dirInfo.date.isEmpty() && dirInfo.dirsize > 0) {
				return tr("%1 - %2 kB")
					.arg(dirInfo.date)
					.arg(dirInfo.dirsize/1024);
			} else if (dirInfo.dirsize > 0) {
				return tr("%1 kB").arg(dirInfo.dirsize/1024);
			}
		}
		return QVariant();
	}
	return _fsModel.data(fsIndex, role);
}

void EnduranceDirectoryModel::clearEnduranceData()
{
	if (_clearInProgress)
		return;
	_clearInProgress = true;
	emit clearingChanged();
	_directories.clear();
	emit totalSizeMBChanged();
	_dirInfoQueue.clear();
	QFuture<void> future = QtConcurrent::run(doClearEnduranceData, &_dirInfoWatcher);
	_clearWatcher.setFuture(future);
}

QModelIndex
EnduranceDirectoryModel::index(int row, int column, const QModelIndex &parent) const
{
	qDebug() << Q_FUNC_INFO << "row:" << row << ",column:" << column << ",parent:" << parent;
	Q_UNUSED(parent);
	Q_UNUSED(column);
	return createIndex(row, 0);
}

// TODO: Allow removal of single endurance directories.
/*
bool EnduranceDirectoryModel::rmdir(const QModelIndex &index) const
{
	qDebug() << Q_FUNC_INFO;
	return _fsModel.rmdir(index);
}
*/

void EnduranceDirectoryModel::slotDirInfoResultsReadyAt(int beginIndex, int endIndex)
{
	qDebug() << Q_FUNC_INFO << ":" << beginIndex << "-" << endIndex;
	for (int i=beginIndex; i < endIndex; ++i) {
		DirectoryInfo result = _dirInfoWatcher.resultAt(i);
		qDebug() << Q_FUNC_INFO << ":" << result.dirname << ":" << result.date << ":" << (result.dirsize/1024);
		_directories[result.dirname] = result;
	}
	emit totalSizeMBChanged();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0));
	_dirInfoWatcherBusy = false;
	kickDirInfoWatcher();
}

void EnduranceDirectoryModel::clearFinished()
{
	qDebug() << Q_FUNC_INFO;
	_clearInProgress = false;
	emit clearingChanged();
}

QStringList EnduranceDirectoryModel::directoryList() const
{
	qDebug() << Q_FUNC_INFO;
	QStringList result;
	int rows = rowCount();
	for (int row=0; row < rows; ++row) {
		QVariant rowData = data(index(row), TitleRole);
		QString entry = rowData.toString();
		if (!entry.isEmpty())
			result << entry;
	}
	return result;
}

int EnduranceDirectoryModel::totalSizeMB() const
{
	quint64 totalSize = 0;
	foreach (const DirectoryInfo &dirInfo, _directories)
		totalSize += dirInfo.dirsize;
	return totalSize / (1024*1024);
}

QModelIndex EnduranceDirectoryModel::fsRootIndex() const
{
	return _fsModel.index(SNAPSHOTDIR, 0);
}
