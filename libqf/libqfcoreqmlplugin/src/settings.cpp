#include "settings.h"

#include <qf/core/log.h>

using namespace qf::core;

Settings::Settings(QObject *parent) :
	Super(parent)
{
	qfLogFuncFrame() << parent;
}

Settings::~Settings()
{
	qfLogFuncFrame() << this;
}
