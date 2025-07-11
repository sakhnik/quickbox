#pragma once

#include <QString>

namespace Event::services::qx {

struct RunChange
{
	std::optional<QString> class_name;
	std::optional<QString> first_name;
	std::optional<QString> last_name;
	std::optional<QString> registration;
	std::optional<int> si_id;
	// std::optional<bool> si_id_rent;
	QString note;

	// static RunChange fromJsonString(const QString &json);
	static RunChange fromVariantMap(const QVariantMap &map);
};
struct OrigRunRecord
{
	QString first_name;
	QString last_name;
	QString registration;
	int si_id;
	// bool si_id_rent;

	QVariantMap toVariantMap() const;
};

// struct ChangesRecord
// {
//     int64_t id;
//     QString source;
//     QString data_type,
//     int64_t data_id;
//     data: ChangeData,
//     user_id: Option<String>,
//     status: Option<ChangeStatus>,
//     created: QxDateTime,
//     lock_number: Option<i64>,
// };

} // namespace Event::services::qx



