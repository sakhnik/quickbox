#include "desktoputils.h"

#include <QApplication>
#include <QScreen>



namespace qf::qmlwidgets::internal {

QRect DesktopUtils::moveRectToVisibleDesktopScreen(const QRect &rect)
{
	QScreen *scr = QApplication::screenAt(rect.topLeft());
	if(!scr) {
		scr = QApplication::primaryScreen();
	}
	if(!scr) {
		return {};
	}
	QRect screen_rect = scr->geometry();
	if(screen_rect.contains(rect.topLeft())) {
		return rect;
	}
			auto ret = rect;
		ret.moveCenter(screen_rect.center());
		return ret;

}

} // namespace qf::qmlwidgets::internal



