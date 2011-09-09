#include "EnduranceDaemon.h"
#include "EnduranceDaemonIfAdaptor.h"
#include <QCoreApplication>
#include <QDBusConnection>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	EnduranceDaemon daemon;
	new EnduranceDaemonIfAdaptor(&daemon);
	QDBusConnection connection = QDBusConnection::sessionBus();
	connection.registerService("com.nokia.EnduranceDaemon");
	connection.registerObject("/", &daemon);
	return app.exec();
}
