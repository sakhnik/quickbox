#ifndef DLGEDITCONNECTION_H
#define DLGEDITCONNECTION_H

#include "servertreeitem.h"

#include "ui_dlgeditconnection.h"

class DlgEditConnection : public QDialog, private Ui::DlgEditConnection
{
	Q_OBJECT
public:
	enum OptionIndex {OptionIndexMySql = 0, OptionIndexSqlite};
public:
	DlgEditConnection(QWidget *parent = nullptr);
private slots:
	void onOkClicked();
	void onCancelClicked() {reject();}
	void onDriverCurrentIndexChanged(const QString &s);
	void onFindDatabaseFileClicked();
protected:
	QStringList drivers;
	QStringList codecs;
	static QStringList mysqlCodecs;
	Connection::Params m_connectionParams;
public:
	void setParams(const Connection::Params &params);
	const Connection::Params& params();
};

#endif // DLGEDITCONNECTION_H
