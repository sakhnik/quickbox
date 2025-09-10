#ifndef RUNSTABLEITEMDELEGATE_H
#define RUNSTABLEITEMDELEGATE_H

#include "../../Classes/src/classesplugin.h"
#include <quickevent/gui/og/itemdelegate.h>

#include <qf/core/utils.h>

class RunsTableItemDelegate : public quickevent::gui::og::ItemDelegate
{
	Q_OBJECT
private:
	typedef quickevent::gui::og::ItemDelegate Super;
public:
	RunsTableItemDelegate(qf::gui::TableView * parent = nullptr);

	QF_PROPERTY_BOOL_IMPL2(s, S, tartTimeHighlightVisible, false)

	void setHighlightedClassId(int class_id, int stage_id);
	void reloadHighlightedClassId();
protected:
	void paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
	int m_stageId = 0;
	int m_highlightedClassId = 0;
	Classes::ClassDef m_classDef;
};

#endif // RUNSTABLEITEMDELEGATE_H
