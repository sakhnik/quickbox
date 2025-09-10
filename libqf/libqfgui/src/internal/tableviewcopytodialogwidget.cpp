#include "tableviewcopytodialogwidget.h"
#include "ui_tableviewcopytodialogwidget.h"
#include "../tableview.h"

#include <qf/core/log.h>
#include <qf/gui/model/tablemodel.h>

namespace qfm = qf::gui::model;
using namespace qf::gui::internal;

TableViewCopyToDialogWidget::TableViewCopyToDialogWidget(QWidget *parent)
	: Super(parent)
{
	ui = new Ui::TableViewCopyToDialogWidget;
	ui->setupUi(this);

	auto *m = new qfm::TableModel(this);
	//m->setElideDisplayedTextAt(0);
	//qfTrash() << QF_FUNC_NAME << "model created:" << f_model;
	tableView()->setTableModel(m);
}

TableViewCopyToDialogWidget::~TableViewCopyToDialogWidget()
{
	delete ui;
}
/*
QFTableViewWidget* TableViewCopyToDialogWidget::tableViewWidget()
{
	return ui->tableViewWidget;
}
*/
qf::gui::TableView* TableViewCopyToDialogWidget::tableView()
{
	return ui->tableView;
}

bool TableViewCopyToDialogWidget::isInsert()
{
	return ui->chkInsertRows->isChecked();
}
