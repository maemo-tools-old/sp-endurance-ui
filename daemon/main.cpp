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
#include "endurancedaemon_adaptor.h"
#include <QCoreApplication>
#include <QDBusConnection>
#include <stdio.h>
#include <syslog.h>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	EnduranceDaemon daemon;
	app.connect(&daemon, SIGNAL(shutdown()), SLOT(quit()));

	new EnduranceDaemonAdaptor(&daemon);
	QDBusConnection connection = QDBusConnection::systemBus();
	if (!connection.isConnected()) {
		syslog(LOG_ERR,
			"EnduranceDaemon ERROR: DBus connection failed.");
		return 1;
	}
	if (!connection.registerService("com.nokia.EnduranceDaemon")) {
		syslog(LOG_ERR,
			"EnduranceDaemon ERROR: unable to register DBus "
			"service 'com.nokia.EnduranceDaemon' (%s).",
			connection.lastError().message().toAscii().data());
		return 1;
	}
	if (!connection.registerObject("/com/nokia/EnduranceDaemon", &daemon)) {
		syslog(LOG_ERR,
			"EnduranceDaemon ERROR: unable to register object "
			"/com/nokia/EnduranceDaemon.\n");
		return 1;
	}
	int rc =  app.exec();
	syslog(LOG_ERR, "exit");
	return rc;
}
