#ifndef SPEAKER_H
#define SPEAKER_H

#include <qf/gui/framework/plugin.h>
#include <qf/gui/framework/ipersistentsettings.h>

namespace qf {

namespace core {  namespace model { class SqlTableModel; }}

namespace gui {
class Action;
namespace framework { class PartWidget; class DockWidget; }
}

}

namespace Speaker {

class SpeakerPlugin : public qf::gui::framework::Plugin, public qf::gui::framework::IPersistentSettings
{
	Q_OBJECT
	using Super = qf::gui::framework::Plugin;
public:
	SpeakerPlugin(QObject *parent = nullptr);
private:
	void onInstalled();
// 	void doDbVacuum();
private:
	qf::gui::framework::PartWidget *m_partWidget = nullptr;
};

}

#endif
