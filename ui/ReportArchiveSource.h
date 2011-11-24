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
#ifndef REPORTARCHIVESOURCE_H_
#define REPORTARCHIVESOURCE_H_

#include "EnduranceConstants.h"
#include "ArchiveSource.h"

#include <QDir>

/**
 * The generated report archive source provider.
 */
class ReportArchiveSource : public ArchiveSource
{
	Q_OBJECT

	virtual QStringList contents() const {
		return QDir(REPORTDIR).entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
	}

	QString dataDir() const { return ".sp-endurance"; }

	QString workDir() const { return USERDIR; }

};



#endif /* REPORTARCHIVESOURCE_H_ */
