#pragma once

#include <QWidget>

namespace qf::core::model { class SqlTableModel; }

namespace Event::services::qx {

namespace Ui {
class QxLateRegistrationsWidget;
}

class QxClientService;

class QxLateRegistrationsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit QxLateRegistrationsWidget(QWidget *parent = nullptr);
	~QxLateRegistrationsWidget();

	void onDbEventNotify(const QString &domain, const QVariant &payload);
	void onVisibleChanged(bool is_visible);
private:
	QxClientService* service();
	void reload();
	void addQxChangeRow(int change_sql_id);

	void resizeColumns();
	void showMessage(const QString &msg, bool is_error = false);
	void applyCurrentChange();
private:
	Ui::QxLateRegistrationsWidget *ui;
	qf::core::model::SqlTableModel *m_model;
};

}

