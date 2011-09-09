TEMPLATE = app
TARGET = sp-endurance-ui
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
	EnduranceConstants.h \
	EnduranceDaemon.h \
	EnduranceDaemonIf.h \
	EnduranceReportingBase.h \
	EndurancePlotControl.h \
	EnduranceReportControl.h \
	EnduranceDirectoryModel.h \
	EnduranceArchiver.h
SOURCES += \
	EnduranceDaemon.cpp \
	EnduranceDaemonIf.cpp \
	EnduranceReportingBase.cpp \
	EndurancePlotControl.cpp \
	EnduranceReportControl.cpp \
	EnduranceDirectoryModel.cpp \
	EnduranceArchiver.cpp \
	main.cpp
RESOURCES += qmlfiles.qrc

QT += declarative
DEFINES += QT_NO_DEBUG_OUTPUT
