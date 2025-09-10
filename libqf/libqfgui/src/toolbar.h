#ifndef QF_GUI_TOOLBAR_H
#define QF_GUI_TOOLBAR_H

#include "guiglobal.h"

#include <QToolBar>

namespace qf {
namespace gui {

//class Action;

class QFGUI_DECL_EXPORT ToolBar : public QToolBar
{
	Q_OBJECT
private:
	typedef QToolBar Super;
public:
	ToolBar(QWidget *parent = nullptr);
	~ToolBar() Q_DECL_OVERRIDE;
};

}}

#endif // QF_GUI_TOOLBAR_H

