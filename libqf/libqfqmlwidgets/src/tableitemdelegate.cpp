#include "tableitemdelegate.h"

#include "style.h"
#include "tableview.h"

#include <qf/core/assert.h>

#include <QPainter>

using namespace qf::qmlwidgets;

TableItemDelegate::TableItemDelegate(TableView *parent) :
	Super(parent)
{
}

TableView * TableItemDelegate::view() const
{
	auto *view = qobject_cast<TableView*>(parent());
	QF_ASSERT_EX(view != nullptr, "Cannot use TableItemDelegate without TableView parent");
	return view;
}

void TableItemDelegate::paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	TableView *v = view();
	if(v) {
		QModelIndex ix = v->currentIndex();
		if(index.row() == ix.row() && index != ix) {
			/// fill current row background
			/// da se to udelat i takhle bez patchovani QT
			/// pozor, aby to fungovalo musi se jeste v TableView::currentChanged() volat updateRow() na radky u kterych se meni selekce
			static const auto sel_row_background1 = isDarkTheme()? QColor(0x707010): QColor(245, 245, 184); // khaki
			static const auto sel_row_background2 = isDarkTheme()? QColor(0x284125): QColor(245, 245, 184); // lime
			painter->fillRect(option.rect, (v->inlineEditSaveStrategy() == TableView::OnEditedValueCommit)? sel_row_background2: sel_row_background1);
		}
		else {
			/// fill background of RO cells
			Qt::ItemFlags flags = index.flags();
			//qfInfo() << "col:" << index.column() << "editable:" << f.contains(Qt::ItemIsEditable);
			if(!(flags & Qt::ItemIsEditable)) {
				static const auto ro_cell_background = isDarkTheme()? QColor(0x00264d):  QColor(0xeeeeff);
				painter->fillRect(option.rect, ro_cell_background);
			}
		}
	}
}

void TableItemDelegate::paintForeground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Super::paint(painter, option, index);
}

void TableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	paintBackground(painter, option, index);
	paintForeground(painter, option, index);
}

QWidget *TableItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QWidget *ret = Super::createEditor(parent, option, index);
	return ret;
}
