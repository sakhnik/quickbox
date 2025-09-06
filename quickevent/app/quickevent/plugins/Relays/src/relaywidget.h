#ifndef RELAYWIDGET_H
#define RELAYWIDGET_H

#include <qf/qmlwidgets/framework/datadialogwidget.h>

namespace quickevent::gui::og { class SqlTableModel; }

namespace Ui { class  RelayWidget; }

class  RelayWidget : public qf::qmlwidgets::framework::DataDialogWidget
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::framework::DataDialogWidget Super;
public:
	explicit  RelayWidget(QWidget *parent = nullptr);
	~RelayWidget() Q_DECL_OVERRIDE;

	bool load(const QVariant &id = QVariant(), int mode = qf::qmlwidgets::model::DataDocument::ModeEdit) Q_DECL_OVERRIDE;
private:
	Q_SLOT bool loadLegsTable();
	//Q_SLOT bool saveLegsTable();
	//void onRunsTableCustomContextMenuRequest(const QPoint &pos);
	bool saveData() Q_DECL_OVERRIDE;
	void checkLegsStartTimes();

	void addLeg();
	void removeLeg();
	void moveLegUp();
	void moveLegDown();
private:
	Ui:: RelayWidget *ui;
	quickevent::gui::og::SqlTableModel *m_legsModel;
};

#endif // RELAYWIDGET_H
