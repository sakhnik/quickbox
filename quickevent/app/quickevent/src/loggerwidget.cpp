#include "loggerwidget.h"
#include "application.h"

#include <qf/gui/log.h>

#include <qf/gui/model/logtablemodel.h>

LoggerWidget::LoggerWidget(QWidget *parent)
	: Super(parent)
{
	addCategoryActions(tr("<empty>"), QString(), NecroLog::Level::Info);

	m_logModel = new qf::gui::model::LogTableModel(this);
	connect(Application::instance(), &Application::newLogEntry, m_logModel, &qf::gui::model::LogTableModel::addLogEntry, Qt::QueuedConnection);
	setLogTableModel(m_logModel);
}

LoggerWidget::~LoggerWidget() = default;

void LoggerWidget::onDockWidgetVisibleChanged(bool visible)
{
	//qfWarning() << "onDockWidgetVisibleChanged" << visible;
	if(visible) {
		checkScrollToLastEntry();
	}
}

