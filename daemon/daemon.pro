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

DbusAdaptor_OBJECTS = EnduranceDaemon.cpp

DbusAdaptorSources.name = Generate DBUS adaptor sources
DbusAdaptorSources.input = DbusAdaptor_OBJECTS
DbusAdaptorSources.output = ${QMAKE_FILE_IN_BASE}IfAdaptor.cpp 
DbusAdaptorSources.commands = @true
DbusAdaptorSources.CONFIG += no_link
DbusAdaptorSources.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += DbusAdaptorSources

DbusAdaptorHeaders.name = Generate DBUS adaptor headers
DbusAdaptorHeaders.input = DbusAdaptor_OBJECTS
DbusAdaptorHeaders.output = ${QMAKE_FILE_IN_BASE}IfAdaptor.h
DbusAdaptorHeaders.commands = qdbusxml2cpp -c ${QMAKE_FILE_IN_BASE}IfAdaptor \
		-a ${QMAKE_FILE_IN_BASE}IfAdaptor com.nokia.${QMAKE_FILE_IN_BASE}.xml
DbusAdaptorHeaders.CONFIG += no_link
DbusAdaptorHeaders.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += DbusAdaptorHeaders


QT += dbus
