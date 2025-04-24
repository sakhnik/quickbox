#pragma once

#include <QAbstractTableModel>
#include <QObject>

namespace Event::services::qx {

class QxClientService;

class QxLateRegistrationsModel : public QAbstractTableModel
{
	Q_OBJECT
	using Super = QAbstractTableModel;

public:
	enum Column {ColRunId, ColChange, ColUserId, ColStatus, ColCount};

	explicit QxLateRegistrationsModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &) const override;
	int columnCount(const QModelIndex &) const override { return ColCount; }
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	QVariant data(const QModelIndex &index, int role) const override;

	void reload();
	QVariant value(int row, int col) const;
	Q_SIGNAL void modelLoadInfo(QString, bool is_error);
private:
	QxClientService* service();
private:
	QVariantList m_records;
};

}
