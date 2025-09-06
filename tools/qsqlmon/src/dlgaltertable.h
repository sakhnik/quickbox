//
// C++ Interface: dlgaltertable
//
// Description: 
//
//
// Author: Fanda Vacek <fanda.vacek@volny.cz>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DLGALTERTABLE_H
#define DLGALTERTABLE_H

#include "ui_dlgaltertable.h"

#include <qf/core/sql/catalog.h>

#include <QDialog>

class MainWindow;

/**
@author Fanda Vacek
*/
class DlgAlterTable : public QDialog, Ui::DlgAlterTable
{
	Q_OBJECT
	friend class DlgIndexDef;
private:
	typedef QDialog Super;
public:
	DlgAlterTable(QWidget * parent, const QString& db, const QString& tbl);
	virtual ~DlgAlterTable();

	void accept() Q_DECL_OVERRIDE;
protected:
	void refresh();
private:
	void onFieldEdit_clicked();
	void onFieldInsert_clicked(bool append = false);
	void onFieldAppend_clicked();
	void onFieldDelete_clicked();
	void onIndexAdd_clicked();
	void onIndexEdit_clicked();
	void onIndexDelete_clicked();

protected:
	MainWindow* mainWindow();
	QSqlDatabase connection();
	bool execCommand(const QString &qs);
	QString dropIndexCommand(const QString &index_name);
private:
	QString m_tableName, m_schemaName;
	QString oldComment;
};

#endif
