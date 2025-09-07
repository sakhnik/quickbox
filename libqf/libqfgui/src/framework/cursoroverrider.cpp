#include "cursoroverrider.h"

#include <QApplication>

using namespace qf::gui::framework;

CursorOverrider::CursorOverrider(Qt::CursorShape cursor_shape)
{
	QApplication::setOverrideCursor(cursor_shape);
}

CursorOverrider::~CursorOverrider()
{
	QApplication::restoreOverrideCursor();
}
