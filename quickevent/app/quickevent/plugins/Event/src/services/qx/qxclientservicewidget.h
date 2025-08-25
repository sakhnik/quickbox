#pragma once

#include <qf/qmlwidgets/framework/dialogwidget.h>

namespace Event::services::qx {

namespace Ui {
class QxClientServiceWidget;
}

class QxClientService;

class QxClientServiceWidget : public qf::qmlwidgets::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::qmlwidgets::framework::DialogWidget;
public:
	explicit QxClientServiceWidget(QWidget *parent = nullptr);
	~QxClientServiceWidget() override;
private:
	enum class MessageType { Ok, Error, Progress };
	void setMessage(const QString &msg = {}, MessageType msg_type = MessageType::Ok);
	QxClientService* service();
	bool saveSettings();
	void updateOCheckListPostUrl();
	void testConnection();
	void exportEventInfo();
	void exportStartList();
	void exportRuns();
private:
	Ui::QxClientServiceWidget *ui;
	bool acceptDialogDone(int result) override;
};

}

