
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef QF_GUI_REPORTS_REPORTPROCESSORCONTEXT_H
#define QF_GUI_REPORTS_REPORTPROCESSORCONTEXT_H

#include "../../guiglobal.h"
#include "../../graphics/stylecache.h"

#include <QSharedData>
#include <QVariantMap>

namespace qf {
namespace gui {
namespace reports {

//! TODO: write class documentation.
class QFGUI_DECL_EXPORT ReportProcessorContext
{
private:
	struct Data : public QSharedData
	{
		qf::gui::graphics::StyleCache styleCache;
		//QVariantMap options;
		//KeyVals keyVals;
	};
	QSharedDataPointer<Data> d;
	class SharedDummyHelper {};
	ReportProcessorContext(SharedDummyHelper);
	static const ReportProcessorContext& sharedNull();
public:
	bool isNull() const {return d == sharedNull().d;}
	const qf::gui::graphics::StyleCache& styleCache() const {return d->styleCache;}
	qf::gui::graphics::StyleCache& styleCacheRef() {return d->styleCache;}
	void clear();
public:
	ReportProcessorContext();
	/// protoze je qf::gui::graphics::StyleCache explicitne sdilena, potrebuju konstruktor s jinou cachi, nez ma null() context, protoze jinak se styleCache chova jako staticka promenna
	/// vsechny contexty pouzivaji stejnou cache a to tu, kterou zalozi null context
	//ReportProcessorContext(const StyleCache &style_cache);
};

}}}

#endif // QF_GUI_REPORTS_REPRTPROCESSORCONTEXT_H

