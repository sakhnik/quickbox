#include "timeedit.h"

#include <quickevent/core/og/timems.h>

#include <qf/core/log.h>



namespace quickevent::gui::og {

TimeEdit::TimeEdit(QWidget *parent)
	: Super(parent)
{
	qfLogFuncFrame();
}

TimeEdit::~TimeEdit()
= default;

core::og::TimeMs TimeEdit::timeMs() const
{
	core::og::TimeMs t = core::og::TimeMs::fromString(text());
	return t;

}

void TimeEdit::setTimeMs(const core::og::TimeMs &t)
{
	qfLogFuncFrame();
	if(!(timeMs() == t)) {
		setText(t.toString());
		emit timeMsChanged();
	}
}

}
