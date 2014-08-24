
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef ITEMVALUEEDITORWIDGET_H
#define ITEMVALUEEDITORWIDGET_H

#include "../../framework/dialogwidget.h"

namespace qf {
namespace qmlwidgets {
namespace reports {

namespace Ui {class ItemValueEditorWidget;};

//! TODO: write class documentation.
class  ItemValueEditorWidget : public qf::qmlwidgets::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::framework::DialogWidget Super;
private:
	Ui::ItemValueEditorWidget *ui;
public:
	QVariant value() const;
	void setValue(const QVariant &val);
public:
	ItemValueEditorWidget(QWidget *parent = NULL);
	virtual ~ItemValueEditorWidget();
};

}}}

#endif // ITEMVALUEEDITORWIDGET_H

