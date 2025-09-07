#ifndef CLASSDEFWIDGET_H
#define CLASSDEFWIDGET_H

#include <qf/gui/framework/datadialogwidget.h>

namespace Ui {
class ClassDefWidget;
}

class ClassDefWidget : public qf::gui::framework::DataDialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DataDialogWidget Super;
public:
	ClassDefWidget(QWidget *parent = nullptr);
	~ClassDefWidget() Q_DECL_OVERRIDE;

	bool load(const QVariant &id = QVariant(), int mode = qf::gui::model::DataDocument::ModeEdit) Q_DECL_OVERRIDE;
private:
	Ui::ClassDefWidget *ui;
};

#endif // CLASSDEFWIDGET_H
