#pragma once

#include <QString>
#include <QVariant>
#include <QDateTime>

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

enum class DataType {
	OcChange,
	RunUpdateRequest,
	RunUpdated,
	RadioPunch,
	CardReadout,
};
enum class ChangeStatus {
	Pending,
	Locked,
	Accepted,
	Rejected,
};
struct EventChange
{
	int64_t id;
	QString source;
	DataType data_type;
	int64_t data_id;
	QVariant data;
	QString user_id;
	ChangeStatus status;
	QString status_message;
	QDateTime created;
	int64_t lock_number;

	QVariantMap toVariantMap() const;
};

// struct ChangeRecord
// {
// 	int64_t id;
// 	QString source;
// 	QString data_type,
// 	int64_t data_id;
// 	data: ChangeData,
// 	user_id: Option<String>,
// 	status: Option<ChangeStatus>,
// 	created: QxDateTime,
// 	lock_number: Option<i64>,
// };

} // namespace Event::services::qx



