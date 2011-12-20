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

#ifndef ENDURANCEPLOTCONTROL_H
#define ENDURANCEPLOTCONTROL_H

#include "EnduranceReportingBase.h"

class EndurancePlotControl : public EnduranceReportingBase
{
	Q_OBJECT

public:
	EndurancePlotControl(QObject *parent = 0);

public slots:
	void generate(int first, int next);
	void open();

private slots:
	void runnerFinished(int, QProcess::ExitStatus);
};

#endif /* ENDURANCEPLOTCONTROL_H */
