#include "stage.h"

#include <QTimeZone>

using namespace Event;

StageData::StageData(const QVariantMap &data)
	: Super(data)
{
}

QDateTime StageData::startDateTime() const
{
	auto dt = value(QStringLiteral("startdatetime")).toDateTime();
	dt = dt.toTimeZone(QTimeZone::systemTimeZone());
	return dt;
}




