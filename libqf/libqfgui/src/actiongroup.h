#ifndef QF_GUI_ACTIONGROUP_H
#define QF_GUI_ACTIONGROUP_H

#include "guiglobal.h"

#include <QActionGroup>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT ActionGroup : public QActionGroup
{
	Q_OBJECT
private:
	typedef QActionGroup Super;
public:
	ActionGroup(QObject *parent = nullptr);

	Q_INVOKABLE QAction* addAction(QAction *action) {return Super::addAction(action);}
};

} // namespace gui
} // namespace qf

#endif // QF_GUI_ACTIONGROUP_H
