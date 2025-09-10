#ifndef QF_GUI_EXPORTCSVTABLEVIEWWIDGET_H
#define QF_GUI_EXPORTCSVTABLEVIEWWIDGET_H

#include "exporttableviewwidget.h"

namespace qf {
namespace gui {

class ExportCsvTableViewWidget : public qf::gui::ExportTableViewWidget
{
	Q_OBJECT
private:
	typedef qf::gui::ExportTableViewWidget Super;
public:
	ExportCsvTableViewWidget(QTableView *table_view, QWidget *parent = nullptr);
protected:
	QVariant exportOptions() const Q_DECL_OVERRIDE;
	void setExportOptions(const QVariant& opts) const Q_DECL_OVERRIDE;
};

} // namespace gui
} // namespace qf

#endif // QF_GUI_EXPORTCSVTABLEVIEWWIDGET_H
