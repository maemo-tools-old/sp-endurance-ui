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
			"service 'com.nokia.EnduranceDaemon' (%s).", connection.lastError().message().toAscii().data());
		return 1;
	}
	if (!connection.registerObject("/com/nokia/EnduranceDaemon", &daemon)) {
		syslog(LOG_ERR,
			"EnduranceDaemon ERROR: unable to register root "
			"object.\n");
		return 1;
	}
	int rc =  app.exec();
	syslog(LOG_ERR, "exit");
	return rc;
}
