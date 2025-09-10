#ifndef EDITCODESWIDGET_H
#define EDITCODESWIDGET_H

#include <qf/gui/framework/dialogwidget.h>

namespace Ui {
class EditCodesWidget;
}

namespace qf::gui::model { class SqlTableModel; }

class EditCodesWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	explicit EditCodesWidget(QWidget *parent = 0);
	~EditCodesWidget() Q_DECL_OVERRIDE;
private:
	Ui::EditCodesWidget *ui;
	qf::gui::model::SqlTableModel *m_tableModel;
};

#endif // EDITCODESWIDGET_H
