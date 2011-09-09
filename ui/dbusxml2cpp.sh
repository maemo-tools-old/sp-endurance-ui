#!/bin/sh
echo "Generating EnduranceDaemonIf.h & EnduranceDaemonIf.cpp ..."
qdbusxml2cpp -c EnduranceDaemonIf -p EnduranceDaemonIf ../daemon/com.nokia.EnduranceDaemon.xml
RET=$?
if [ $RET -ne 0 ] ; then
	echo "ERROR: generation failed (ret=$RET)."
	exit $RET
fi
echo "OK"
