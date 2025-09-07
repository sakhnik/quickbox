#include "speakerplugin.h"
#include "speakerwidget.h"

#include <qf/gui/framework/mainwindow.h>
#include <qf/gui/dialogs/messagebox.h>
#include <qf/gui/action.h>
#include <qf/gui/menubar.h>

#include <qf/core/sql/query.h>
#include <qf/core/log.h>

namespace qff = qf::gui::framework;
namespace qfw = qf::gui;
using ::PartWidget;

namespace Speaker {

SpeakerPlugin::SpeakerPlugin(QObject *parent)
	: Super("Speaker", parent), qf::gui::framework::IPersistentSettings(this)
{
	setPersistentSettingsId("SpeakerPlugin");
	connect(this, &SpeakerPlugin::installed, this, &SpeakerPlugin::onInstalled, Qt::QueuedConnection);
}

void SpeakerPlugin::onInstalled()
{
	qfLogFuncFrame();
	m_partWidget = qff::initPluginWidget<SpeakerWidget, PartWidget>(tr("&Speaker"), featureId());
}

}
