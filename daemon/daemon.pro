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

target.path = $$PREFIX/sbin

services.path = /usr/share/dbus-1/system-services
services.files = com.nokia.EnduranceDaemon.service

startup.path = /etc/init/apps
startup.files = com.nokia.EnduranceDaemon.conf

dbus.path = /etc/dbus-1/system.d
dbus.files = EnduranceDaemon.conf

INSTALLS += target services startup dbus

DISTCLEANFILES = Makefile

DEFINES += QT_NO_DEBUG_OUTPUT
