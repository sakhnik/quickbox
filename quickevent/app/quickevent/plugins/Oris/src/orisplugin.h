#pragma once

#include <qf/gui/framework/plugin.h>
//#include <qf/gui/framework/ipersistentsettings.h>

class OrisImporter;
class TxtImporter;
class XmlImporter;

namespace Oris {

class OrisPlugin : public qf::gui::framework::Plugin//, public qf::gui::framework::IPersistentSettings
{
	Q_OBJECT
	using Super = qf::gui::framework::Plugin;
public:
	OrisPlugin(QObject *parent = nullptr);
private:
	void onInstalled();
private:
	OrisImporter *m_orisImporter = nullptr;
	TxtImporter *m_txtImporter = nullptr;
	XmlImporter *m_xmlImporter = nullptr;
};

}
