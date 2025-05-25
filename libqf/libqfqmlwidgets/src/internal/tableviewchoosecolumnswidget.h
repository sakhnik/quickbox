#ifndef QF_QMLWIDGETS_INTERNAL_TABLEVIEWCHOOSECOLUMNSWIDGET_H
#define QF_QMLWIDGETS_INTERNAL_TABLEVIEWCHOOSECOLUMNSWIDGET_H

#include <QWidget>
//#include <QVariantList>

class QStandardItemModel;

namespace qf {

namespace core {
namespace model {
class TableModel;
}
}

namespace qmlwidgets {
namespace internal {

namespace Ui {
class TableViewChooseColumnsWidget;
}

class TableViewChooseColumnsWidget : public QWidget
{
	Q_OBJECT
public:
	TableViewChooseColumnsWidget(QWidget *parent = nullptr);
	~TableViewChooseColumnsWidget() Q_DECL_OVERRIDE;
protected:
	QStandardItemModel *f_exportedColumnsModel;
public:
	void loadColumns(qf::core::model::TableModel *model);
protected slots:
	void onColumnsAllClicked();
	void onColumnsNoneClicked();
	void onColumnsInvertClicked();
	void onColumnsAppendSelectedClicked();
	void onColumnsRemoveSelectedClicked();
	void onColumnShiftUpClicked();
	void onColumnShiftDownClicked();
public:
	/// [{caption:"",origin:("table" | "model"),index:(col_ix | fld_ix)}, ...]
	QVariantList exportedColumns() const;
	void setExportedColumns(const QVariant &cols);
private:
	Ui::TableViewChooseColumnsWidget *ui;
};

}}}

#endif // QF_QMLWIDGETS_INTERNAL_TABLEVIEWCHOOSECOLUMNSWIDGET_H

