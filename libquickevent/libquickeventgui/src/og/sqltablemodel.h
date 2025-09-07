#ifndef QUICKEVENTGUI_OG_SQLTABLEMODEL_H
#define QUICKEVENTGUI_OG_SQLTABLEMODEL_H

#include "../quickeventguiglobal.h"

#include <qf/gui/model/sqltablemodel.h>

namespace quickevent {
namespace gui {
namespace og {

class QUICKEVENTGUI_DECL_EXPORT SqlTableModel : public qf::gui::model::SqlTableModel
{
	Q_OBJECT
private:
	typedef qf::gui::model::SqlTableModel Super;
public:
	SqlTableModel(QObject *parent = nullptr);
	~SqlTableModel();
public:
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
protected:
	QVariant rawValueToEdit(int column_index, const QVariant &val) const Q_DECL_OVERRIDE;
	QVariant editValueToRaw(int column_index, const QVariant &val) const Q_DECL_OVERRIDE;
};

}}}

#endif // QUICKEVENTGUI_OG_SQLTABLEMODEL_H
