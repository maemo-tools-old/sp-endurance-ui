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
	Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
	EnduranceDirectoryModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	bool clearing() const { return _clearInProgress; }
	QStringList directoryList() const;
	int totalSizeMB() const;

public slots:
	void clearEnduranceData();
	int rowCount() const;
	//bool rmdir(const QModelIndex &index) const;

signals:
	void clearingChanged();
	void totalSizeMBChanged();
	void rowCountChanged();

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
