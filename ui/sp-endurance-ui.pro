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

DbusAdaptor_OBJECTS = EnduranceDaemon.cpp

DbusAdaptorSources.name = Generate DBUS adaptor sources
DbusAdaptorSources.input = DbusAdaptor_OBJECTS
DbusAdaptorSources.output = ${QMAKE_FILE_IN_BASE}If.cpp 
DbusAdaptorSources.commands = @true
DbusAdaptorSources.CONFIG += no_link
DbusAdaptorSources.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += DbusAdaptorSources

DbusAdaptorHeaders.name = Generate DBUS adaptor headers
DbusAdaptorHeaders.input = DbusAdaptor_OBJECTS
DbusAdaptorHeaders.output = ${QMAKE_FILE_IN_BASE}If.h
DbusAdaptorHeaders.commands = qdbusxml2cpp -c ${QMAKE_FILE_IN_BASE}If \
		-p ${QMAKE_FILE_IN_BASE}If ../daemon/com.nokia.${QMAKE_FILE_IN_BASE}.xml
DbusAdaptorHeaders.CONFIG += no_link
DbusAdaptorHeaders.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += DbusAdaptorHeaders


QT += declarative
DEFINES += QT_NO_DEBUG_OUTPUT
