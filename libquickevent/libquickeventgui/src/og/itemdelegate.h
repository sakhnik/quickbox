#ifndef QUICKEVENTGUI_OG_ITEMDELEGATE_H
#define QUICKEVENTGUI_OG_ITEMDELEGATE_H

#include "../quickeventguiglobal.h"

#include <qf/gui/sqltableitemdelegate.h>

class QItemEditorCreatorBase;

namespace quickevent {
namespace gui {
namespace og {

class QUICKEVENTGUI_DECL_EXPORT ItemDelegate : public qf::gui::SqlTableItemDelegate
{
	Q_OBJECT
private:
	typedef qf::gui::SqlTableItemDelegate Super;
public:
	ItemDelegate(qf::gui::TableView * parent = nullptr);
	~ItemDelegate() Q_DECL_OVERRIDE;
private:
	QItemEditorFactory *m_factory = nullptr;
};

}}}

#endif // QUICKEVENTGUI_OG_ITEMDELEGATE_H
