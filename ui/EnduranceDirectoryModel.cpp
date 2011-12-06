/*
 * This file is part of sp-endurance-ui package.
 *
 * Copyright (C) 2011 by Nokia Corporation
 *
 * Contact: Eero Tamminen <eero.tamminen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
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

static void
directoryInfoFor(QSharedPointer<DirectoryInfo> dirInfo)
{
	qDebug() << Q_FUNC_INFO << ":" << dirInfo->dirname;

	/* We get notifications at the moment a new endurance directory is
	 * created, and while save-incremental-endurance-stats is still
	 * running. Check directory modified timestamps to heuristically detect
	 * when the script is done.
	 */
	for (int i=0; i < 10; ++i) {
		struct stat statbuf;
		if (stat((SNAPSHOTDIR+dirInfo->dirname).toUtf8(), &statbuf) == -1)
			break;
		QDateTime now = QDateTime::currentDateTime();
		QDateTime modified;
		modified.setTime_t(statbuf.st_mtime);
		if (modified.secsTo(now) > 5)
			break;
		sleep(1);
	}
	dirInfo->dirsize = recursiveSize(SNAPSHOTDIR+dirInfo->dirname);
	dirInfo->date = snapshotDirectoryDate(SNAPSHOTDIR+dirInfo->dirname);
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
doClearEnduranceData(QFutureWatcher<void> *_dirInfoWatcher)
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

	QDir().mkpath(SNAPSHOTDIR);


	_fsWatcher.addPath(SNAPSHOTDIR);
	connect(&_fsWatcher, SIGNAL(directoryChanged(const QString &)),
			this, SLOT(slotDirectoryChanged(const QString &)));

	slotDirectoryChanged(SNAPSHOTDIR);

	connect(&_dirInfoWatcher, SIGNAL(finished()),
		this, SLOT(dirInfoFinished()));
	connect(&_clearWatcher, SIGNAL(finished()),
		this, SLOT(clearFinished()));
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


int EnduranceDirectoryModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return rowCount();
}

int EnduranceDirectoryModel::rowCount() const
{
	int ret = _directories.size();
	//qDebug() << Q_FUNC_INFO << ":" << ret;
	return ret;
}

QVariant EnduranceDirectoryModel::data(const QModelIndex &index, int role) const
{
	qDebug() << Q_FUNC_INFO << "index:" << index << ", role:" << role;
	if (index.row() >= 0 && index.row() < _directories.size()) {
		const QSharedPointer<DirectoryInfo> &dirInfo = _directories[index.row()];
		if (role == TitleRole) {
			return dirInfo->dirname;
		}
		if (role == SubtitleRole) {
			if (!dirInfo->isEmpty()) {
				qDebug() << Q_FUNC_INFO << "SubtitleRole: got dirInfo!";
				if (!dirInfo->date.isEmpty() && dirInfo->dirsize > 0) {
					return tr("%1 - %2 kB")
						.arg(dirInfo->date)
						.arg(dirInfo->dirsize/1024);
				} else if (dirInfo->dirsize > 0) {
					return tr("%1 kB").arg(dirInfo->dirsize/1024);
				}
			}
		}
	}
	return QVariant();
}

void EnduranceDirectoryModel::clearEnduranceData()
{
	if (_clearInProgress)
		return;
	_clearInProgress = true;
	emit clearingChanged();
	beginRemoveRows(QModelIndex(), 0, _directories.size() - 1);
	_directories.clear();
	endRemoveRows();
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

void EnduranceDirectoryModel::dirInfoFinished()
{
	qDebug() << Q_FUNC_INFO;
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
	foreach (QSharedPointer<DirectoryInfo> dirInfo, _directories)
		result << dirInfo->dirname;
	return result;
}

int EnduranceDirectoryModel::totalSizeMB() const
{
	quint64 totalSize = 0;
	foreach (QSharedPointer<DirectoryInfo> dirInfo, _directories)
		totalSize += dirInfo->dirsize;
	return totalSize / (1024*1024);
}


void EnduranceDirectoryModel::slotDirectoryChanged(const QString &path)
{
	Q_UNUSED(path);
	QDir dir(SNAPSHOTDIR);
	QStringList filter;
	filter << "[0-9][0-9][0-9]" <<
			  "[0-9][0-9][0-9][0-9]" <<
			  "[0-9][0-9][0-9][0-9][0-9]";

	QStringList snapshot = dir.entryList(filter,
			QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

	int iDir = 0, iSnap = 0;
	while (iDir < _directories.size() && iSnap < snapshot.size()) {
		int equals = _directories[iDir]->dirname.compare(snapshot[iSnap]);
		if (equals == 0) {
			iDir++;
			iSnap++;
		}
		else if (equals > 0) {
			insertDir(iDir, snapshot[iSnap]);
		}
		else {
			removeDir(iDir);
		}
	}
	if (iDir == _directories.size()) {
		while (iSnap < snapshot.size()) {
			insertDir(_directories.size(), snapshot[iSnap]);
			iSnap++;
		}
	}
	else {
		while (iDir < _directories.size()) {
			removeDir(iDir);
			iDir++;
		}
	}
}

void EnduranceDirectoryModel::insertDir(int index, const QString &dirName)
{
	beginInsertRows(QModelIndex(), index, index);
	QSharedPointer<DirectoryInfo> dirInfo(new DirectoryInfo(dirName));
	_directories.insert(index, dirInfo);
	_dirInfoQueue.append(dirInfo);
	kickDirInfoWatcher();
	endInsertRows();
	emit rowCountChanged();
}

void EnduranceDirectoryModel::removeDir(int index)
{
	beginRemoveRows(QModelIndex(), index, index);
	_directories.remove(index);
	endRemoveRows();
	emit rowCountChanged();
	emit totalSizeMBChanged();
}

