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

#include "EnduranceDaemon.h"
#include "EndurancePlotControl.h"
#include "EnduranceReportControl.h"
#include "EnduranceDirectoryModel.h"
#include "EnduranceArchiver.h"
#include "EnduranceLogReader.h"
#include "DataArchiverOptions.h"
#include "ReportArchiverOptions.h"
#include <QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QDebug>

#ifdef USE_BOOSTER
#include <MDeclarativeCache>
#include <MNotification>
#define EXPORT_MAIN Q_DECL_EXPORT
#else
#define EXPORT_MAIN
#endif

EXPORT_MAIN int main(int argc, char **argv)
{
	QApplication *app = NULL;
	QDeclarativeView *window = NULL;
#ifdef USE_BOOSTER
	app = MDeclarativeCache::qApplication(argc, argv);
	window = MDeclarativeCache::qDeclarativeView();
#else
	app = new QApplication(argc, argv);
	window = new QDeclarativeView();
#endif
	app->setProperty("NoMStyle", true);
	qmlRegisterType<EnduranceDaemon>("com.nokia.endurance", 1, 0, "EnduranceDaemon");
	//qmlRegisterUncreatableType<EnduranceReportingBase>("com.nokia.endurance", 1, 0, "EnduranceReportingBase", "Create EndurancePlotControl or EnduranceReportControl instead.");
	qmlRegisterType<EndurancePlotControl>("com.nokia.endurance", 1, 0, "EndurancePlotControl");
	qmlRegisterType<EnduranceReportControl>("com.nokia.endurance", 1, 0, "EnduranceReportControl");
	qmlRegisterType<EnduranceDirectoryModel>("com.nokia.endurance", 1, 0, "EnduranceDirectoryModel");
	qmlRegisterType<EnduranceArchiver>("com.nokia.endurance", 1, 0, "EnduranceArchiver");
	qmlRegisterType<EnduranceLogReader>("com.nokia.endurance", 1, 0, "EnduranceLogReader");
	qmlRegisterType<ArchiverOptions>("com.nokia.endurance", 1, 0, "ArchiverOptions");
	qmlRegisterType<DataArchiverOptions>("com.nokia.endurance", 1, 0, "DataArchiverOptions");
	qmlRegisterType<ReportArchiverOptions>("com.nokia.endurance", 1, 0, "ReportArchiverOptions");
	//QDir::setCurrent(app.applicationDirPath());
	window->setSource(QUrl("qrc:/qmldir/main.qml"));
	window->showFullScreen();
	int rc = app->exec();

	EnduranceDaemon ed;
	bool stopped = ed.tryShutdown();

#ifdef MAEMO6
	if (!stopped) {
		MNotification notification(
			QLatin1String("x-nokia.sp-endurance-ui"),
			QObject::tr("Endurance daemon"),
			QObject::tr("Periodic endurance data collection continues in the background"));
		notification.publish();
	}
#endif

	return stopped;

	return rc;
}
