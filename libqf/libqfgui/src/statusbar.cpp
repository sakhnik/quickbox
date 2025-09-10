#include "statusbar.h"

#include <qf/core/log.h>

using namespace qf::gui;

StatusBar::StatusBar(QWidget *parent) :
	Super(parent)
{
}

qf::gui::StatusBar::~StatusBar()
{
	qfLogFuncFrame() << this;
}

void StatusBar::showProgress(const QString &msg, int completed, int total)
{
	Q_UNUSED(completed)
	Q_UNUSED(total)
	showMessage(msg);
}

void StatusBar::showMessage(const QString &message, int timeout)
{
	Super::showMessage(message, timeout);
}

void StatusBar::addPermanentWidget(QWidget *widget, int stretch)
{
	widget->setParent(nullptr);
	Super::addPermanentWidget(widget, stretch);
}

void StatusBar::addWidget(QWidget *widget, int stretch)
{
	widget->setParent(nullptr);
	Super::addWidget(widget, stretch);
}
