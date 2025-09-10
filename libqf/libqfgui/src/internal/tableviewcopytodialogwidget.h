#ifndef QF_GUI_INTERNAL_TABLEVIEWCOPYTODIALOGWIDGET_H
#define QF_GUI_INTERNAL_TABLEVIEWCOPYTODIALOGWIDGET_H


#include "../framework/dialogwidget.h"

namespace qf {
namespace gui {
class TableView;

namespace internal {

namespace Ui {class TableViewCopyToDialogWidget;}

class  TableViewCopyToDialogWidget : public framework::DialogWidget
{
	Q_OBJECT
private:
	typedef framework::DialogWidget Super;
public:
	TableViewCopyToDialogWidget(QWidget *parent = nullptr);
	~TableViewCopyToDialogWidget() Q_DECL_OVERRIDE;
public:
	//QFTableViewWidget* tableViewWidget();
	TableView* tableView();
	bool isInsert();
private:
	Ui::TableViewCopyToDialogWidget *ui;
};

}}}

#endif // QF_GUI_INTERNAL_TABLEVIEWCOPYTODIALOGWIDGET_H

