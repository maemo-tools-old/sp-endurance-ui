#ifndef ENDURANCEREPORTCONTROL_H
#define ENDURANCEREPORTCONTROL_H

#include "EnduranceReportingBase.h"
#include <QProcess>

class EnduranceReportControl : public EnduranceReportingBase
{
	Q_OBJECT

public:
	EnduranceReportControl(QObject *parent = 0);

public slots:
	void generate();
	void open();

private slots:
	void runnerFinished(int, QProcess::ExitStatus);
};

#endif /* ENDURANCEREPORTCONTROL_H */
