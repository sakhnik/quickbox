#ifndef EVENT_STAGEWIDGET_H
#define EVENT_STAGEWIDGET_H

#include <qf/gui/framework/datadialogwidget.h>

#include <QWidget>

namespace Event {

namespace Ui {
class StageWidget;
}

class StageWidget : public qf::gui::framework::DataDialogWidget
{
	Q_OBJECT
private:
	using Super = qf::gui::framework::DataDialogWidget;
public:
	explicit StageWidget(QWidget *parent = 0);
	~StageWidget();

	bool load(const QVariant &id = QVariant(), int mode = qf::gui::model::DataDocument::ModeEdit) Q_DECL_OVERRIDE;
protected:
	bool saveData() Q_DECL_OVERRIDE;
private:
	Ui::StageWidget *ui;
};

}

#endif
