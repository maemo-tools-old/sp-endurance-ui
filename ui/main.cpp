#include "EnduranceDaemon.h"
#include "EndurancePlotControl.h"
#include "EnduranceReportControl.h"
#include "EnduranceDirectoryModel.h"
#include "EnduranceArchiver.h"
#include <QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <applauncherd/MDeclarativeCache>

Q_DECL_EXPORT int main(int argc, char **argv)
{
    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
	app->setProperty("NoMStyle", true);
	qmlRegisterType<EnduranceDaemon>("com.nokia.endurance", 1, 0, "EnduranceDaemon");
	//qmlRegisterUncreatableType<EnduranceReportingBase>("com.nokia.endurance", 1, 0, "EnduranceReportingBase", "Create EndurancePlotControl or EnduranceReportControl instead.");
	qmlRegisterType<EndurancePlotControl>("com.nokia.endurance", 1, 0, "EndurancePlotControl");
	qmlRegisterType<EnduranceReportControl>("com.nokia.endurance", 1, 0, "EnduranceReportControl");
	qmlRegisterType<EnduranceDirectoryModel>("com.nokia.endurance", 1, 0, "EnduranceDirectoryModel");
	qmlRegisterType<EnduranceArchiver>("com.nokia.endurance", 1, 0, "EnduranceArchiver");
	//QDir::setCurrent(app.applicationDirPath());
	QDeclarativeView *window = MDeclarativeCache::qDeclarativeView();
	window->setSource(QUrl("qrc:/qmldir/main.qml"));
	window->showFullScreen();
	return app->exec();
}
