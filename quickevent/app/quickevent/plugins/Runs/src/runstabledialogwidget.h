#ifndef RUNSTABLEDIALOGWIDGET_H
#define RUNSTABLEDIALOGWIDGET_H

#include <QWidget>

#include <qf/gui/framework/dialogwidget.h>

namespace Ui {
class RunsTableDialogWidget;
}

class RunsTableWidget;

class RunsTableDialogWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	explicit RunsTableDialogWidget(QWidget *parent = 0);
	~RunsTableDialogWidget();

	RunsTableWidget* runsTableWidget();
	void reload(int stage_id, int class_id, bool show_offrace = false, const QString &sort_column = QString(), int select_competitor_id = 0);
private:
	Ui::RunsTableDialogWidget *ui;
};

#endif // RUNSTABLEDIALOGWIDGET_H
