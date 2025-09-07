#include "compiledtextstyle.h"

using namespace qf::gui::reports::style;

//==========================================================
//           CompiledTextStyle
//==========================================================
const CompiledTextStyle &CompiledTextStyle::sharedNull()
{
	static CompiledTextStyle n = CompiledTextStyle(SharedDummyHelper());
	return n;
}

CompiledTextStyle::CompiledTextStyle(CompiledTextStyle::SharedDummyHelper)
{
	d = new Data();
}

CompiledTextStyle::CompiledTextStyle()
{
	*this = sharedNull();
}

CompiledTextStyle::CompiledTextStyle(const QFont &f)
{
	d = new Data;
	d->font = f;
}
