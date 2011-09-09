#ifndef ENDURANCEPLOTCONTROL_H
#define ENDURANCEPLOTCONTROL_H

#include "EnduranceReportingBase.h"

class EndurancePlotControl : public EnduranceReportingBase
{
	Q_OBJECT

public:
	EndurancePlotControl(QObject *parent = 0);

public slots:
	void generate();
	void open();

private slots:
	void runnerFinished(int, QProcess::ExitStatus);
};

#endif /* ENDURANCEPLOTCONTROL_H */
