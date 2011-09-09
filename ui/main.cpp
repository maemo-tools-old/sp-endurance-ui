#include "EnduranceDaemon.h"
#include "EnduranceDaemonIf.h"
#include "EndurancePlotControl.h"
#include "EnduranceReportControl.h"
#include "EnduranceDirectoryModel.h"
#include "EnduranceArchiver.h"
#include <QApplication>
#include <QtDeclarative>
#include <QUrl>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setProperty("NoMStyle", true);
	qmlRegisterType<EnduranceDaemon>("com.nokia.endurance", 1, 0, "EnduranceDaemon");
	//qmlRegisterUncreatableType<EnduranceReportingBase>("com.nokia.endurance", 1, 0, "EnduranceReportingBase", "Create EndurancePlotControl or EnduranceReportControl instead.");
	qmlRegisterType<EndurancePlotControl>("com.nokia.endurance", 1, 0, "EndurancePlotControl");
	qmlRegisterType<EnduranceReportControl>("com.nokia.endurance", 1, 0, "EnduranceReportControl");
	qmlRegisterType<EnduranceDirectoryModel>("com.nokia.endurance", 1, 0, "EnduranceDirectoryModel");
	qmlRegisterType<EnduranceArchiver>("com.nokia.endurance", 1, 0, "EnduranceArchiver");
	//QDir::setCurrent(app.applicationDirPath());
	QDeclarativeView window;
	window.setSource(QUrl("qrc:/qmldir/main.qml"));
	window.showFullScreen();
	return app.exec();
}
