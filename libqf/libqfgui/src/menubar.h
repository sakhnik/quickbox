#ifndef QF_GUI_MENUBAR_H
#define QF_GUI_MENUBAR_H

#include "guiglobal.h"

#include <QMenuBar>

namespace qf {
namespace gui {

class Action;

class QFGUI_DECL_EXPORT MenuBar : public QMenuBar
{
	Q_OBJECT
private:
	typedef QMenuBar Super;
public:
	explicit MenuBar(QWidget *parent = nullptr);
public:
	Q_INVOKABLE qf::gui::Action* actionForPath(const QString &path, bool create_if_not_exists = true);
};

}}

#endif // QF_GUI_MENUBAR_H
