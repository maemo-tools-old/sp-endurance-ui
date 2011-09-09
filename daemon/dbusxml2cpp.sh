#!/bin/sh
echo "Generating EnduranceDaemonIfAdaptor.h & EnduranceDaemonIfAdaptor.cpp ..."
qdbusxml2cpp -c EnduranceDaemonIfAdaptor -a EnduranceDaemonIfAdaptor com.nokia.EnduranceDaemon.xml
RET=$?
if [ $RET -ne 0 ] ; then
	echo "ERROR: generation failed (ret=$RET)."
	exit $RET
fi
echo "OK"
