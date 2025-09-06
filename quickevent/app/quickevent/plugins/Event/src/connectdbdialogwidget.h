#ifndef CONNECTDBDIALOGWIDGET_H
#define CONNECTDBDIALOGWIDGET_H

#include "eventplugin.h"

#include <qf/qmlwidgets/framework/dialogwidget.h>

namespace Ui {
class ConnectDbDialogWidget;
}

class ConnectDbDialogWidget : public qf::qmlwidgets::framework::DialogWidget
{
	Q_OBJECT
private:
	using Super = qf::qmlwidgets::framework::DialogWidget;
public:
	explicit ConnectDbDialogWidget(QWidget *parent = nullptr);
	~ConnectDbDialogWidget();

	QString eventName();
	Event::EventPlugin::ConnectionType connectionType();
	QString serverHost();
	int serverPort();
	QString serverUser();
	QString serverPassword();
	QString singleWorkingDir();

	void loadSettings();
	void saveSettings();
private:
	void onSingleWorkingDirectoryClicked();
private:
	Ui::ConnectDbDialogWidget *ui;
};

#endif // CONNECTDBDIALOGWIDGET_H
