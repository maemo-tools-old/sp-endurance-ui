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

#ifndef ARCHIVEROPTIONS_H
#define ARCHIVEROPTIONS_H

#include "EnduranceConstants.h"

#include <QObject>
#include <QStringList>

/**
 * The base source file/directory provider for the EnduranceArchiver.
 */
class ArchiverOptions : public QObject
{
	Q_OBJECT

public:
	/**
	 * The list of files/directories to archive.
	 * @return
	 */
	virtual QStringList contents() const { return QStringList(); }

	/**
	 * The archiver working directory.
	 * @return
	 */
	virtual QString workDir() const { return ""; }

	/**
	 * The data directory relative to the working directory,
	 * containing the files/directories returned by contents().
	 * @return
	 */
	virtual QString dataDir() const { return ""; }

	virtual QString outputTemplate() const { return ""; }

	virtual QString outputPath() const { return ARCHIVEDIR; }
};

#endif /* ARCHIVEROPTIONS_H */
