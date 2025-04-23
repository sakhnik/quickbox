#include "qxlateregistrationsmodel.h"

#include "qxclientservice.h"

#include <qf/core/log.h>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

namespace Event::services::qx {

QxLateRegistrationsModel::QxLateRegistrationsModel(QObject *parent)
	: QAbstractTableModel{parent}
{

}

int QxLateRegistrationsModel::rowCount(const QModelIndex &) const
{
	return m_records.size();
}

QVariant QxLateRegistrationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal) {
		if (role == Qt::DisplayRole) {
			switch (static_cast<Column>(section)) {
			case ColRunId: return tr("Run id");
			case ColChange: return tr("Change");
			case ColStatus: return tr("Status");
			case ColUserId: return tr("User");
			case ColCount: return {};
			}
		}
		return {};
	}
	return Super::headerData(section, orientation, role);
}

QVariant QxLateRegistrationsModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole) {
		return value(index.row(), index.column());
	}
	return {};
}

QxClientService *QxLateRegistrationsModel::service()
{
	auto *svc = qobject_cast<QxClientService*>(Event::services::Service::serviceByName(QxClientService::serviceId()));
	Q_ASSERT(svc);
	return svc;
}

QVariant QxLateRegistrationsModel::value(int row, int col) const
{
	auto variant_to_json = [](const QVariant val) {
		auto doc = QJsonDocument::fromVariant(val);
		return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
	};
	if (row < 0 || row >= m_records.size()) {
		return {};
	}
	// qfInfo() << "value:" << row << col << "data:" << variant_to_json(m_records[row]);
	switch (static_cast<Column>(col)) {
	case ColRunId: return m_records[row].toMap().value("run_id");
	case ColChange: {
		return variant_to_json(m_records[row].toMap().value("data").toMap().value("RunUpdateRequest"));
	}
	case ColStatus: return m_records[row].toMap().value("status");
	case ColUserId: return m_records[row].toMap().value("user_id");
	case ColCount: return {};
	}
}

void QxLateRegistrationsModel::reload()
{
	emit modelLoadInfo(tr("Loading changes ..."), false);
	try {
		auto reply = service()->loadChanges("RunUpdateRequest", {});
		connect(reply, &QNetworkReply::finished, this, [reply, this]() {
			beginResetModel();
			m_records.clear();
			QString err;
			if(reply->error()) {
				err = reply->errorString();
				emit modelLoadInfo(err, true);
			}
			else {
				QJsonParseError err;
				auto json = QJsonDocument::fromJson(reply->readAll(), &err);
				if (err.error != QJsonParseError::NoError) {
					emit modelLoadInfo(err.errorString(), true);
				}
				else {
					m_records = json.array().toVariantList();
					emit modelLoadInfo(tr("Loading changes Ok"), false);
				}
			}
			endResetModel();
			reply->deleteLater();
		});
	}
	catch (const qf::core::Exception &e) {
		emit modelLoadInfo(e.message(), true);
	}

}

}
