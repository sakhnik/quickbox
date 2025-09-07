#ifndef QF_GUI_INTERNAL_DESKTOPUTILS_H
#define QF_GUI_INTERNAL_DESKTOPUTILS_H

class QRect;

namespace qf {
namespace gui {
namespace internal {

class DesktopUtils
{
public:
	static QRect moveRectToVisibleDesktopScreen(const QRect &rect);
};

} // namespace internal
} // namespace gui
} // namespace qf

#endif // QF_GUI_INTERNAL_DESKTOPUTILS_H
