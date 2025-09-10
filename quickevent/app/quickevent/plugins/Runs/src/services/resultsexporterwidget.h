#pragma once

#include <qf/gui/framework/dialogwidget.h>

namespace Runs {
namespace services {

namespace Ui {
class ResultsExporterWidget;
}

class ResultsExporter;

class ResultsExporterWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::gui::framework::DialogWidget;
public:
	explicit ResultsExporterWidget(QWidget *parent = nullptr);
	~ResultsExporterWidget();
private:
	void onBtChooseExportDirClicked();
	void onBtExportResultsClicked();
	bool saveSettings();
	bool acceptDialogDone(int result) override;
	ResultsExporter* service();
private:
	Ui::ResultsExporterWidget *ui;
};

}}

