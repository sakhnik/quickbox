#ifndef QF_GUI_MENU_H
#define QF_GUI_MENU_H

not used for now

#include "guiglobal.h"

#include <QMenu>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT Menu : public QMenu
{
	Q_OBJECT
private:
	typedef QMenu Super;
public:
	explicit Menu(QWidget *parent = 0);
public:
};

}}

#endif // MENU_H
