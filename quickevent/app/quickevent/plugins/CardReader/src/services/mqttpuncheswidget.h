#pragma once

#include <qf/gui/framework/dialogwidget.h>

namespace CardReader {
namespace services {

namespace Ui {
class MqttPunchesWidget;
}

class MqttPunchesWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT

	using Super = qf::gui::framework::DialogWidget;
public:
	explicit MqttPunchesWidget(QWidget *parent = nullptr);
	~MqttPunchesWidget();

private:
	Ui::MqttPunchesWidget *ui;
};


}} // namespace services
