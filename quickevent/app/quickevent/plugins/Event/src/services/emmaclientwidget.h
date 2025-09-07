#pragma once

#include <qf/gui/framework/dialogwidget.h>

namespace Event {
namespace services {

namespace Ui {
class EmmaClientWidget;
}

class EmmaClient;

class EmmaClientWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::gui::framework::DialogWidget;
public:
	explicit EmmaClientWidget(QWidget *parent = nullptr);
	~EmmaClientWidget();
private:
	void onBtChooseExportDirClicked();
	void onBtExportSplitsTxtClicked();
	void onBtExportFinishTxtClicked();
	void onBtExportStartListTxtClicked();
	void onBtExportResultsXml30Clicked();
	void onBtExportStartListXml30Clicked();
	bool acceptDialogDone(int result) override;
	EmmaClient* service();
	bool saveSettings();
private:
	Ui::EmmaClientWidget *ui;
};

}}

