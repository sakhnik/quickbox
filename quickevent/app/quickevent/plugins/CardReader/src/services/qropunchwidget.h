#pragma once

#include <qf/gui/framework/dialogwidget.h>

namespace CardReader {
namespace services {

namespace Ui {
	class QrOPunchWidget;
}

class QrOPunch;

class QrOPunchWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::gui::framework::DialogWidget;
public:
	explicit QrOPunchWidget(QWidget *parent = nullptr);
	~QrOPunchWidget() override;
private:
	bool acceptDialogDone(int result) override;
	QrOPunch* service();
	void saveSettings();
	void onBtChooseLogFileClicked();
private:
	Ui::QrOPunchWidget *ui;
};

}}
