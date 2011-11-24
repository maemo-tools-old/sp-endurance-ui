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
#ifndef DATAARCHIVESOURCE_H_
#define DATAARCHIVESOURCE_H_

#include "EnduranceDirectoryModel.h"
#include "ArchiveSource.h"
#include "EnduranceConstants.h"

#include <QObject>

/**
 * The collected snapshot data archive source provider.
 */
class DataArchiveSource : public ArchiveSource
{
	Q_OBJECT

	Q_PROPERTY(EnduranceDirectoryModel *model
			READ model
			WRITE setModel)

public:
	EnduranceDirectoryModel *model() { return _model; }
	void setModel(EnduranceDirectoryModel *model) { _model = model; }

	virtual QStringList contents() const {
		if (!_model) return QStringList();
		return _model->directoryList();
	}

	QString dataDir() const { return ".endurance"; }

	QString workDir() const { return DATADIR; }

private:
	EnduranceDirectoryModel *_model;
};

#endif /* DATAARCHIVESOURCE_H_ */
