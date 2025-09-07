#include "qfqmlreports_plugin.h"

#include <qf/gui/reports/processor/reportitempara.h>
#include <qf/gui/reports/processor/reportitemimage.h>
#include <qf/gui/reports/processor/reportitemframe.h>
#include <qf/gui/reports/processor/reportitemdetail.h>
#include <qf/gui/reports/processor/reportitemband.h>
#include <qf/gui/reports/processor/banddatamodel.h>
#include <qf/gui/reports/processor/reportitembreak.h>
#include <qf/gui/reports/processor/reportitemreport.h>
#include <qf/gui/reports/processor/style/sheet.h>
#include <qf/gui/reports/processor/style/color.h>
#include <qf/gui/reports/processor/style/pen.h>

#include <QtQml>

QT_BEGIN_NAMESPACE

void QFQmlReportPlugin::registerTypes(const char *uri)
{
	Q_ASSERT(uri == QLatin1String("qf.qmlreports"));

	qmlRegisterType<qf::gui::reports::ReportItem>(uri, 1, 0, "Item");
	qmlRegisterType<qf::gui::reports::ReportItemReport>(uri, 1, 0, "Report");
	qmlRegisterType<qf::gui::reports::ReportItemFrame>(uri, 1, 0, "Frame");
	qmlRegisterType<qf::gui::reports::ReportItemFrame>(uri, 1, 0, "Space");
	qmlRegisterType<qf::gui::reports::BandDataModel>(uri, 1, 0, "DataModel");
	qmlRegisterType<qf::gui::reports::ReportItemBand>(uri, 1, 0, "Band");
	qmlRegisterType<qf::gui::reports::ReportItemDetail>(uri, 1, 0, "Detail");
	qmlRegisterType<qf::gui::reports::ReportItemPara>(uri, 1, 0, "Para");
	qmlRegisterType<qf::gui::reports::ReportItemImage>(uri, 1, 0, "Image");
	qmlRegisterType<qf::gui::reports::ReportItemBreak>(uri, 1, 0, "Break");
	qmlRegisterType<qf::gui::reports::style::Sheet>(uri, 1, 0, "StyleSheet");
	qmlRegisterType<qf::gui::reports::style::Color>(uri, 1, 0, "Color");
	qmlRegisterType<qf::gui::reports::style::Pen>(uri, 1, 0, "Pen");
	qmlRegisterType<qf::gui::reports::style::Brush>(uri, 1, 0, "Brush");
	qmlRegisterType<qf::gui::reports::style::Font>(uri, 1, 0, "Font");
	qmlRegisterType<qf::gui::reports::style::Text>(uri, 1, 0, "TextStyle");

	// Auto-increment the import to stay in sync with ALL future QtQuick minor versions
	qmlRegisterModule(uri, 1, 0);
}

QT_END_NAMESPACE
