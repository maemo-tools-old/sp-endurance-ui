#include "EnduranceDaemon.h"
#include "EnduranceDaemonIfAdaptor.h"
#include <QCoreApplication>
#include <QDBusConnection>
#include <stdio.h>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	EnduranceDaemon daemon;
	new EnduranceDaemonIfAdaptor(&daemon);
	QDBusConnection connection = QDBusConnection::sessionBus();
	if (!connection.isConnected()) {
		fprintf(stderr,
			"EnduranceDaemon ERROR: DBus connection failed.\n");
		return 1;
	}
	if (!connection.registerService("com.nokia.EnduranceDaemon")) {
		fprintf(stderr,
			"EnduranceDaemon ERROR: unable to register DBus "
			"service 'com.nokia.EnduranceDaemon'.\n");
		return 1;
	}
	if (!connection.registerObject("/", &daemon)) {
		fprintf(stderr,
			"EnduranceDaemon ERROR: unable to register root "
			"object.\n");
		return 1;
	}
	return app.exec();
}
