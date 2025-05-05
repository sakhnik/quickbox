#ifndef COMPETITORS_COMPETITORSPLUGIN_H
#define COMPETITORS_COMPETITORSPLUGIN_H

#include <qf/qmlwidgets/framework/plugin.h>

#include <qf/core/utils.h>

namespace qf::qmlwidgets::framework { class PartWidget; class DockWidget; }

namespace Competitors {

class CompetitorsPlugin : public qf::qmlwidgets::framework::Plugin
{
	Q_OBJECT
	Q_PROPERTY(qf::qmlwidgets::framework::PartWidget* partWidget READ partWidget FINAL)
private:
	typedef qf::qmlwidgets::framework::Plugin Super;
public:
	CompetitorsPlugin(QObject *parent = nullptr);
	~CompetitorsPlugin() Q_DECL_OVERRIDE;

	qf::qmlwidgets::framework::PartWidget *partWidget() {return m_partWidget;}

	// Q_INVOKABLE QObject* createCompetitorDocument(QObject *parent);

	// Q_SIGNAL int editCompetitorOnPunch(int siid);
	// Q_SIGNAL void competitorEdited(); // used to clear caches with competitors

	// Q_SIGNAL void nativeInstalled();

	Q_SLOT void onInstalled();
private:
	qf::qmlwidgets::framework::PartWidget *m_partWidget = nullptr;
};

}

#endif // COMPETITORS_COMPETITORSPLUGIN_H
