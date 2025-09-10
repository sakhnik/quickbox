#pragma once

#include <qf/gui/framework/dialogwidget.h>

namespace CardReader {
namespace services {

namespace Ui {
class RacomClientWidget;
}

class RacomClient;

class RacomClientWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::gui::framework::DialogWidget;
public:
	explicit RacomClientWidget(QWidget *parent = nullptr);
	~RacomClientWidget() override;
private:
	bool acceptDialogDone(int result) override;
	RacomClient* service();
	void saveSettings();
	void onBtChooseTxtSplitsFileClicked();
private:
	Ui::RacomClientWidget *ui;
};

}}

