#ifndef TABLEVIEWWIDGET_H
#define TABLEVIEWWIDGET_H

#include <qf/gui/tableview.h>

#include <QWidget>

namespace Ui {
class TableViewWidget;
}

class  TableView : public qf::gui::TableView
{
	Q_OBJECT
private:
	typedef qf::gui::TableView Super;
public:
	TableView(QWidget *parent = nullptr);
};

class TableViewWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TableViewWidget(QWidget *parent = nullptr);
	~TableViewWidget();
public:
	qf::gui::TableView* tableView();
	Q_SLOT void updateStatus();
	Q_SLOT void setInfo(const QString &info);

	Q_SIGNAL void statusBarAction(const QString &status_text);
private:

private:
	Ui::TableViewWidget *ui;
};

#endif // TABLEVIEWWIDGET_H
