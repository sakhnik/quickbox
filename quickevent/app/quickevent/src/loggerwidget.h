#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <qf/gui/framework/logwidget.h>

namespace qf { namespace core { class LogEntryMap; }}

class LoggerLogDevice;

class LoggerWidget : public qf::gui::framework::LogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::LogWidget Super;
public:
	explicit LoggerWidget(QWidget *parent = 0);
	~LoggerWidget() Q_DECL_OVERRIDE;

	void onDockWidgetVisibleChanged(bool visible) Q_DECL_OVERRIDE;
private:
	qf::gui::model::LogTableModel *m_logModel = nullptr;
};

#endif // LOGGERWIDGET_H
