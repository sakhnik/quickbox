#ifndef DRAWING_DRAWINGGANTTWIDGET_H
#define DRAWING_DRAWINGGANTTWIDGET_H

#include <qf/gui/framework/dialogwidget.h>

class QLineEdit;
class QCheckBox;

namespace drawing {

namespace Ui {
class DrawingGanttWidget;
}

class GanttScene;

class DrawingGanttWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	explicit DrawingGanttWidget(QWidget *parent = nullptr);
	~DrawingGanttWidget() override;

	void settleDownInDialog(qf::gui::dialogs::Dialog *dlg) Q_DECL_OVERRIDE;

	void load(int stage_id);
private slots:
	void onActSaveTriggered();
	void onActFindTriggered();
private:
	Ui::DrawingGanttWidget *ui;
	QLineEdit *m_edFind = nullptr;
	GanttScene *m_ganttScene;
};

}

#endif
