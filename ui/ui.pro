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
	EnduranceArchiver.h \
	EnduranceLogReader.h \
	ArchiveSource.h \
	DataArchiveSource.h \
	ReportArchiveSource.h
	
SOURCES += \
	EnduranceDaemon.cpp \
	EnduranceReportingBase.cpp \
	EndurancePlotControl.cpp \
	EnduranceReportControl.cpp \
	EnduranceDirectoryModel.cpp \
	EnduranceArchiver.cpp \
	EnduranceLogReader.cpp \
	main.cpp
	 
RESOURCES += qmlfiles.qrc

DBUS_INTERFACES = ../daemon/com.nokia.EnduranceDaemon.xml

QT += declarative dbus
#DEFINES += QT_NO_DEBUG_OUTPUT

target.path = $$PREFIX/bin

desktop.path = $$PREFIX/share/applications
desktop.CONFIG += no_check_exist

icon.path = $$PREFIX/share/icons
icon.files = sp-endurance-ui.png

event.path = $$PREFIX/share/meegotouch/notifications/eventtypes
event.files = x-nokia.sp-endurance-ui.conf

INSTALLS += target desktop icon event

DISTCLEANFILES = Makefile

CONFIG += meegotouch

DESKTOP_IN_SRC = sp-endurance-ui.desktop.in
desktop_in.name= Adding applauncher support to .desktop
desktop_in.input = DESKTOP_IN_SRC
desktop_in.output = ${QMAKE_FILE_IN_BASE}
desktop_in.commands = cp ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
desktop_in.CONFIG = no_link target_predeps
desktop_in.variable_out = desktop.files

!$$MAEMO6 {
	message("Building with Maemo 6 features.")
	USE_BOOSTER=1
	DEFINES += MAEMO6
}

!$$USE_BOOSTER {
	message("Building with booster enabled.")
	CONFIG += qdeclarative-boostable
	DEFINES += USE_BOOSTER
	desktop_in.commands = sed "'s/Exec=/Exec=\\/usr\\/bin\\/invoker --single-instance --type=d /'" ${QMAKE_FILE_IN} > ${QMAKE_FILE_OUT} 
}

QMAKE_EXTRA_COMPILERS += desktop_in

PRE_TARGETDEPS += sp-endurance-ui.desktop




