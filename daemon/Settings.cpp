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

#include "Settings.h"

static const QLatin1String KEY_PERIODICCOLLECTIONACTIVE("periodicCollectionActive");
static const QLatin1String KEY_SNAPSHOTINTERVALINMINUTES("snapshotIntervalInMinutes");

Settings::Settings()
	: QSettings("/var/lib/enduranced/enduranced.ini", QSettings::IniFormat)
{
}

bool Settings::periodicCollectionActive() const
{
	return value(KEY_PERIODICCOLLECTIONACTIVE, QVariant(false)).toBool();
}

void Settings::setPeriodicCollectionActive(bool value)
{
	setValue(KEY_PERIODICCOLLECTIONACTIVE, value);
}

int Settings::snapshotIntervalInMinutes() const
{
	return value(KEY_SNAPSHOTINTERVALINMINUTES, QVariant(-1)).toInt();
}

void Settings::setSnapshotIntervalInMinutes(int value)
{
	setValue(KEY_SNAPSHOTINTERVALINMINUTES, value);
}
