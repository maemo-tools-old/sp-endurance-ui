#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : QSettings
{
	Q_OBJECT

public:
	Settings();

	bool periodicCollectionActive() const;
	void setPeriodicCollectionActive(bool);

	int snapshotIntervalInMinutes() const;
	void setSnapshotIntervalInMinutes(int);
};

#endif /* SETTINGS_H */
