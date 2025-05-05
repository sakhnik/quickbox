#include "competitorsplugin.h"
#include "competitordocument.h"
#include "competitorwidget.h"
#include "competitorswidget.h"

#include <qf/qmlwidgets/framework/application.h>
#include <qf/qmlwidgets/framework/mainwindow.h>
#include <qf/qmlwidgets/framework/dockwidget.h>
#include <qf/qmlwidgets/dialogs/dialog.h>
#include <qf/qmlwidgets/action.h>
#include <qf/qmlwidgets/menubar.h>

#include <qf/core/model/sqltablemodel.h>
#include <qf/core/log.h>
#include <qf/core/assert.h>
#include <plugins/Event/src/eventplugin.h>
#include <plugins/Core/src/coreplugin.h>

#include <QQmlEngine>

namespace qfw = qf::qmlwidgets;
namespace qff = qf::qmlwidgets::framework;
namespace qfd = qf::qmlwidgets::dialogs;
namespace qfm = qf::core::model;
namespace qfs = qf::core::sql;
using ::PartWidget;
using qff::getPlugin;
using Event::EventPlugin;

namespace Competitors {

CompetitorsPlugin::CompetitorsPlugin(QObject *parent)
	: Super("Competitors", parent)
{
	connect(this, &CompetitorsPlugin::installed, this, &CompetitorsPlugin::onInstalled);
}

CompetitorsPlugin::~CompetitorsPlugin()
{
	//if(m_registrationsDockWidget)
	//	m_registrationsDockWidget->savePersistentSettingsRecursively();
}

// QObject *CompetitorsPlugin::createCompetitorDocument(QObject *parent)
// {
// 	CompetitorDocument *ret = new CompetitorDocument(parent);
// 	return ret;
// }

void CompetitorsPlugin::onInstalled()
{
	// qff::MainWindow *fwk = qff::MainWindow::frameWork();
	m_partWidget = qff::initPluginWidget<CompetitorsWidget, PartWidget>(tr("&Competitors"), featureId());
	// emit nativeInstalled();
}

}
