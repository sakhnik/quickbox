
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COLUMNSELECTORWIDGET_H
#define COLUMNSELECTORWIDGET_H


#include <qf/gui/framework/dialogwidget.h>

class QSqlDatabase;
namespace Ui {class ColumnSelectorWidget;}


class  ColumnSelectorWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
	Q_PROPERTY(QString persistentSettingsId READ persistentSettingsId WRITE setPersistentSettingsId)
private:
	typedef qf::gui::framework::DialogWidget Super;
	Ui::ColumnSelectorWidget *ui;
protected:
	QString m_tableName;
protected slots:
	void lazyInit();
	void onAll_clicked();
	void onInvert_clicked();
	void onPasteSelectedColumns_clicked();
signals:
	void columnNamesCopiedToClipboard(const QString &column_names);
public:
	ColumnSelectorWidget(QString table_name, const QSqlDatabase &conn, QWidget *parent = nullptr);
	virtual ~ColumnSelectorWidget();
};

#endif // COLUMNSELECTORWIDGET_H

