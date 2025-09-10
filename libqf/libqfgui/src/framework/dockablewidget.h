#ifndef QF_GUI_FRAMEWORK_DOCKABLEWIDGET_H
#define QF_GUI_FRAMEWORK_DOCKABLEWIDGET_H

#include "dialogwidget.h"

namespace qf {
namespace gui {
namespace framework {

class QFGUI_DECL_EXPORT DockableWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	DockableWidget(QWidget *parent = nullptr);
};

} // namespace framework
} // namespace gui
} // namespace qf

#endif // QF_GUI_FRAMEWORK_DOCKABLEWIDGET_H
