TEMPLATE = app
TARGET = enduranced
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
	EnduranceDaemon.h \
	EnduranceDaemonIfAdaptor.h \
	SnapshotterLog.h \
	Settings.h
SOURCES += \
	EnduranceDaemon.cpp \
	EnduranceDaemonIfAdaptor.cpp \
	Settings.cpp \
	SnapshotterLog.cpp \
	main.cpp

QT += dbus
