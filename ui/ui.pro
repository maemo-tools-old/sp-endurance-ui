TEMPLATE = app
TARGET = sp-endurance-ui
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
	EnduranceConstants.h \
	EnduranceDaemon.h \
	EnduranceReportingBase.h \
	EndurancePlotControl.h \
	EnduranceReportControl.h \
	EnduranceDirectoryModel.h \
	EnduranceArchiver.h
SOURCES += \
	EnduranceDaemon.cpp \
	EnduranceReportingBase.cpp \
	EndurancePlotControl.cpp \
	EnduranceReportControl.cpp \
	EnduranceDirectoryModel.cpp \
	EnduranceArchiver.cpp \
	main.cpp
RESOURCES += qmlfiles.qrc

DBUS_INTERFACES = ../daemon/com.nokia.EnduranceDaemon.xml

QT += declarative dbus
#DEFINES += QT_NO_DEBUG_OUTPUT

DISTCLEANFILES = Makefile


