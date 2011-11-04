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
DEFINES += QT_NO_DEBUG_OUTPUT

target.path = $$PREFIX/bin

desktop.path = /usr/share/applications
desktop.files = sp-endurance-ui.desktop

#icon.path = $$PREFIX/share/icons
#icon.files = 

INSTALLS += target desktop
QMAKE_CXXFLAGS += -fPIC
QMAKE_LFLAGS += -pie -rdynamic
LIBS += -lmdeclarativecache

DISTCLEANFILES = Makefile


