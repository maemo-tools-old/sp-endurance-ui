#include "Settings.h"

static const QLatin1String KEY_PERIODICCOLLECTIONACTIVE("periodicCollectionActive");
static const QLatin1String KEY_SNAPSHOTINTERVALINMINUTES("snapshotIntervalInMinutes");

Settings::Settings()
	: QSettings("/var/lib/enduranced/enduranced.ini", QSettings::IniFormat)
{
}

bool Settings::periodicCollectionActive() const
{
	return value(KEY_PERIODICCOLLECTIONACTIVE, QVariant(false)).toBool();
}

void Settings::setPeriodicCollectionActive(bool value)
{
	setValue(KEY_PERIODICCOLLECTIONACTIVE, value);
}

int Settings::snapshotIntervalInMinutes() const
{
	return value(KEY_SNAPSHOTINTERVALINMINUTES, QVariant(-1)).toInt();
}

void Settings::setSnapshotIntervalInMinutes(int value)
{
	setValue(KEY_SNAPSHOTINTERVALINMINUTES, value);
}
