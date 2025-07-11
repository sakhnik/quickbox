#include "runchange.h"

#include <QJsonDocument>
#include <QVariantMap>

namespace Event::services::qx {

// RunChange RunChange::fromJsonString(const QString &json)
// {
// 	RunChange ret;
// 	return ret;
// }

RunChange RunChange::fromVariantMap(const QVariantMap &map)
{
	RunChange ret;

	if (auto v = map.value("class_name"); v.isValid()) { ret.class_name = v.toString(); }
	if (auto v = map.value("first_name"); v.isValid()) { ret.first_name = v.toString(); }
	if (auto v = map.value("last_name"); v.isValid()) { ret.last_name = v.toString(); }
	if (auto v = map.value("registration"); v.isValid()) { ret.registration = v.toString(); }
	if (auto v = map.value("si_id"); v.isValid()) { ret.si_id = v.toInt(); }
	// if (auto v = map.value("si_id_rent"); v.isValid()) { ret.si_id_rent = v.toBool(); }
	ret.note = map.value("note").toString();

	return ret;
}

QVariantMap OrigRunRecord::toVariantMap() const
{
	QVariantMap ret;
	ret["first_name"] = first_name;
	ret["last_name"] = last_name;
	ret["registration"] = registration;
	ret["si_id"] = si_id;
	return ret;
}

} // namespace Event::services::qx


