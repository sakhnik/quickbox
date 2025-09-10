#ifndef QF_GUI_INTERNAL_DLGTABLEVIEWCOPYSPECIAL_H
#define QF_GUI_INTERNAL_DLGTABLEVIEWCOPYSPECIAL_H

#include <QDialog>

#include "../tableview.h"

namespace qf {
namespace gui {
namespace internal {

namespace Ui {class DlgTableViewCopySpecial;}

class DlgTableViewCopySpecial : public QDialog
{
	Q_OBJECT
private:
	typedef QDialog Super;
public:
	DlgTableViewCopySpecial(QWidget *parent = nullptr);
	~DlgTableViewCopySpecial() Q_DECL_OVERRIDE;
public:
	QString fieldsSeparator();
	QString rowsSeparator();
	QString fieldsQuotes();
	qf::gui::TableView::ReplaceEscapes replaceEscapes();
private:
	Ui::DlgTableViewCopySpecial *ui;
};

}}}

#endif // QF_GUI_INTERNAL_DLGTABLEVIEWCOPYSPECIAL_H
