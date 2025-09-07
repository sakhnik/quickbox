#ifndef CLASSESTABLEVIEW_H
#define CLASSESTABLEVIEW_H

#include <qf/gui/tableview.h>

class ClassesTableView : public qf::gui::TableView
{
	Q_OBJECT
private:
	typedef qf::gui::TableView Super;
public:
	ClassesTableView(QWidget *parent = nullptr);

	void insertRow() Q_DECL_OVERRIDE;
	void removeSelectedRows() Q_DECL_OVERRIDE;
};

#endif // CLASSESTABLEVIEW_H
