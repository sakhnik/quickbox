#ifndef QF_GUI_FRAMEWORK_CURSOROVERRIDER_H
#define QF_GUI_FRAMEWORK_CURSOROVERRIDER_H

#include "../guiglobal.h"

#include <Qt>

namespace qf {
namespace gui {
namespace framework {

class QFGUI_DECL_EXPORT CursorOverrider
{
public:
	CursorOverrider(Qt::CursorShape cursor_shape);
	virtual ~CursorOverrider();
};

}}}

#endif // CURSOROVERRIDER_H
