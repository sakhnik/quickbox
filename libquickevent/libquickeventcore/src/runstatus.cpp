#include "runstatus.h"
#include "og/timems.h"
#include <QCoreApplication>

namespace quickevent {
namespace core {

QStringList RunStatus::runsTableColumns()
{
	return {"disqualified", "disqualifiedByOrganizer", "notCompeting", "misPunch", "notStart", "notFinish", "overTime"};
}

RunStatus RunStatus::fromQuery(qf::core::sql::Query &q)
{
	RunStatus new_obj;
	new_obj.m_disqualified = q.value("runs.disqualified").toBool();
	new_obj.m_disqualifiedByOrganizer = q.value("runs.disqualifiedByOrganizer").toBool();
	new_obj.m_notCompeting = q.value("runs.notCompeting").toBool();
	new_obj.m_missingPunch = q.value("runs.misPunch").toBool();
	new_obj.m_didNotStart = q.value("runs.notStart").toBool();
	new_obj.m_didNotFinish = q.value("runs.notFinish").toBool();
	new_obj.m_overTime = q.value("runs.overTime").toBool();
	return new_obj;
}

RunStatus RunStatus::fromTreeTableRow(const qf::core::utils::TreeTableRow &ttr)
{
	RunStatus new_obj;
	new_obj.m_disqualified = ttr.value(QStringLiteral("disqualified")).toBool();
	new_obj.m_disqualifiedByOrganizer = ttr.value("disqualifiedByOrganizer").toBool();
	new_obj.m_notCompeting = ttr.value("notCompeting").toBool();
	new_obj.m_missingPunch = ttr.value("misPunch").toBool();
	new_obj.m_didNotStart = ttr.value("notStart").toBool();
	new_obj.m_didNotFinish = ttr.value("notFinish").toBool();
	new_obj.m_overTime = ttr.value("overTime").toBool();
	return new_obj;
}

bool RunStatus::isOk() const
{
	return !m_disqualified && !m_notCompeting;
}

int RunStatus::ogTime() const
{
	if (m_notCompeting)
		return quickevent::core::og::TimeMs::NOT_COMPETITING_TIME_MSEC;
	if (m_disqualified) {
		if (m_disqualifiedByOrganizer)
			return quickevent::core::og::TimeMs::DISQ_TIME_MSEC;
		if (m_missingPunch)
			return quickevent::core::og::TimeMs::MISPUNCH_TIME_MSEC;
		if (m_didNotStart)
			return quickevent::core::og::TimeMs::NOT_START_TIME_MSEC;
		if (m_didNotFinish)
			return quickevent::core::og::TimeMs::NOT_FINISH_TIME_MSEC;
		if (m_overTime)
			return quickevent::core::og::TimeMs::OVERTIME_TIME_MSEC;
		return quickevent::core::og::TimeMs::DISQ_TIME_MSEC;
	}
	return 0;
}

QString RunStatus::toXmlExportString() const
{
	if (m_notCompeting)
		return QStringLiteral("NotCompeting");
	if (m_disqualified) {
		if (m_disqualifiedByOrganizer)
			return QStringLiteral("Disqualified");
		if (m_missingPunch)
			return QStringLiteral("MissingPunch");
		if (m_didNotStart)
			return QStringLiteral("DidNotStart");
		if (m_didNotFinish)
			return QStringLiteral("DidNotFinish");
		if (m_overTime)
			return QStringLiteral("OverTime");
		return QStringLiteral("Disqualified");
	}
	return QStringLiteral("OK");
}


QString RunStatus::toEmmaExportString() const
{
	if (m_notCompeting)
		return QStringLiteral("NC  ");
	if (m_disqualified)	{
		if (m_disqualifiedByOrganizer)
			return QStringLiteral("DISQ");
		if (m_missingPunch)
			return QStringLiteral("MP  ");
		if (m_didNotStart)
			return QStringLiteral("DNS ");
		if (m_didNotFinish)
			return QStringLiteral("DNF ");
		if (m_overTime)
			return QStringLiteral("OVRT");
		return QStringLiteral("DISQ");
	}
	return QStringLiteral("O.K.");
}

QString RunStatus::toHtmlExportString() const
{
	if (m_notCompeting)
		return QStringLiteral("NC");
	if (m_disqualified)	{
		if (m_disqualifiedByOrganizer)
			return QStringLiteral("DISQ");
		if (m_missingPunch)
			return QStringLiteral("MP");
		if (m_didNotStart)
			return QStringLiteral("DNS");
		if (m_didNotFinish)
			return QStringLiteral("DNF");
		if (m_overTime)
			return QStringLiteral("OVRT");
		return QStringLiteral("DISQ");
	}
	return QStringLiteral("OK");
}

QString RunStatus::toString() const
{
	if (m_notCompeting)
		return tr("NC", "Not Competing");
	if (m_disqualified)	{
		if (m_disqualifiedByOrganizer)
			return tr("DISQ", "Disqualified");
		if (m_missingPunch)
			return tr("MP", "Missing Punch");
		if (m_didNotStart)
			return tr("DNS", "Did Not Start");
		if (m_didNotFinish)
			return tr("DNF", "Did Not Finish");
		if (m_overTime)
			return tr("OVRT", "Over Time");
		return tr("DISQ", "Disqualified");
	}
	return tr("OK");
}

} // namespace core
} // namespace quickevent
