TEMPLATE = app
TARGET = enduranced
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
	EnduranceDaemon.h \
	SnapshotterLog.h \
	Settings.h
SOURCES += \
	EnduranceDaemon.cpp \
	Settings.cpp \
	SnapshotterLog.cpp \
	main.cpp

DBUS_ADAPTORS += com.nokia.EnduranceDaemon.xml

QT += dbus

DISTCLEANFILES = Makefile
