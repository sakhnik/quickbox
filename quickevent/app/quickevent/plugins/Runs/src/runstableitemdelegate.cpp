#include "runstableitemdelegate.h"
#include "runstablemodel.h"

#include <quickevent/core/og/timems.h>

#include <qf/qmlwidgets/tableview.h>
#include <qf/qmlwidgets/framework/mainwindow.h>

#include <qf/core/model/sqltablemodel.h>
#include <qf/core/sql/connection.h>
#include <qf/core/sql/query.h>
#include <qf/core/log.h>
#include <qf/core/assert.h>
#include <plugins/Event/src/eventplugin.h>

#include <QPainter>

namespace qfs = qf::core::sql;
using qf::qmlwidgets::framework::getPlugin;
using Event::EventPlugin;

RunsTableItemDelegate::RunsTableItemDelegate(qf::qmlwidgets::TableView * parent)
	: Super(parent)
{
}

void RunsTableItemDelegate::setHighlightedClassId(int class_id, int stage_id)
{
	//qfWarning() << stage_id << class_id;
	m_stageId = stage_id;
	m_highlightedClassId = class_id;
	reloadHighlightedClassId();
}

void RunsTableItemDelegate::reloadHighlightedClassId()
{
	qf::core::sql::QueryBuilder qb;
	qb.select2("classdefs", "startTimeMin, lastStartTimeMin, startIntervalMin, vacantsBefore, vacantEvery, vacantsAfter")
			.from("classdefs")
			.where("classId=" QF_IARG(m_highlightedClassId));
	bool is_relays = getPlugin<EventPlugin>()->eventConfig()->isRelays();
	if(!is_relays)
		qb.where("stageId=" QF_IARG(m_stageId));
	qfs::Query q(qfs::Connection::forName());
	//qfInfo() << qb.toString();
	q.exec(qb.toString(), qf::core::Exception::Throw);
	if(q.next()) {
		m_classInterval = q.value("startIntervalMin").toInt() * 60 * 1000;
		m_classStartFirst = q.value("startTimeMin").toInt() * 60 * 1000;
		m_classStartLast = q.value("lastStartTimeMin").toInt() * 60 * 1000;
	}
	else {
		m_classInterval = 0;
		m_classStartFirst = 0;
		m_classStartLast = 0;
	}
	//qfInfo() << "interval:" << m_classInterval << "first:" << m_classStartFirst << "last:" << m_classStartLast;
}

void RunsTableItemDelegate::paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Super::paintBackground(painter, option, index);

	qf::qmlwidgets::TableView *v = view();
	if(!v)
		return;

	auto *m = v->model();
	auto *tm = qobject_cast<RunsTableModel*>(v->tableModel());
	if(!(m && tm))
		return;
	bool is_relays = getPlugin<EventPlugin>()->eventConfig()->isRelays();
	if(m_highlightedClassId > 0) {
		if(is_relays) {
			// check that start time in classes is the same
			if(index.column() == RunsTableModel::Columns::col_runs_startTimeMs) {
				QVariant stime_v = m->data(index.sibling(index.row(), RunsTableModel::Columns::col_runs_startTimeMs), Qt::EditRole);
				quickevent::core::og::TimeMs stime = stime_v.value<quickevent::core::og::TimeMs>();
				int start_ms = stime.msec();
				if(!stime.isValid())
					return;
				int leg = m->data(index.sibling(index.row(), RunsTableModel::Columns::col_runs_leg), Qt::EditRole).toInt();
				if(leg == 1 && m_classStartFirst != start_ms) {
					//qfInfo() << m_highlightedClassId << m_classStartFirst << start_ms;
					QColor c = Qt::red;
					c.setAlphaF(0.5);
					painter->fillRect(option.rect, c);
				}
			}
		}
		else if(isStartTimeHighlightVisible() && m_classInterval > 0) {
			//qfInfo() << "col:" << index.column() << m_highlightedClassId << "interval:" << m_classInterval << isStartTimeHighlightVisible();
			QVariant stime_v = m->data(index.sibling(index.row(), RunsTableModel::Columns::col_runs_startTimeMs), Qt::EditRole);
			quickevent::core::og::TimeMs stime = stime_v.value<quickevent::core::og::TimeMs>();
			int start_ms = stime.msec();
			if(!stime.isValid())
				return;
			int prev_start_ms = m_classStartFirst;
			int next_start_ms = m_classStartLast;
			//int table_row = v->toTableModelRowNo(index.row());
			QString club = m->data(index.sibling(index.row(), RunsTableModel::Columns::col_registration), Qt::EditRole).toString().mid(0, 3).trimmed();
			QString prev_club;
			if(index.row() > 0) {
				prev_start_ms = m->data(index.sibling(index.row() - 1, RunsTableModel::Columns::col_runs_startTimeMs), Qt::EditRole).value<quickevent::core::og::TimeMs>().msec();
				prev_club = m->data(index.sibling(index.row() - 1, RunsTableModel::Columns::col_registration), Qt::EditRole).toString().mid(0, 3).trimmed();
			}
			if(index.row() < tm->rowCount() - 1) {
				next_start_ms = m->data(index.sibling(index.row() + 1, RunsTableModel::Columns::col_runs_startTimeMs), Qt::EditRole).value<quickevent::core::og::TimeMs>().msec();
			}

			bool bad_start_time = (start_ms > m_classStartFirst && start_ms == prev_start_ms) || ((start_ms - prev_start_ms) % m_classInterval) != 0;
			bool vacant_before = !bad_start_time && (((start_ms - prev_start_ms) > m_classInterval) || ((index.row() == 0) && (start_ms > m_classStartFirst)));
			bool vacant_after = !bad_start_time
					&& (((next_start_ms - start_ms) > m_classInterval)
						|| ((index.row() == tm->rowCount() - 1) && (start_ms < m_classStartLast)));
			bool bad_club = !vacant_before && (club == prev_club);
			//auto cd = tm->columnDefinition(index.column());
			if(index.column() == RunsTableModel::Columns::col_runs_startTimeMs) {
				//qfWarning() << ix.row() << club << prev_club;
				//qfInfo() << prev_start_ms << start_ms << (start_ms - prev_start_ms) << m_classInterval;
				if(bad_start_time) {
					QColor c = Qt::red;
					c.setAlphaF(0.5);
					painter->fillRect(option.rect, c);
				}
				/*
				else if(vacant_before) {
					QColor c = QColor("lime");
					painter->fillRect(option.rect, c);
				}
				*/
				else if(bad_club) {
					//qfInfo() << (start_ms - prev_start_ms) << m_classInterval;
					QColor c = QColor(Qt::magenta);
					painter->fillRect(option.rect, c);
				}
			}
			static const QColor vacant_color("forestgreen");
			static constexpr int vacant_h_ratio = 20;
			if(vacant_before) {
				QRect r = option.rect;
				int h = r.height() / vacant_h_ratio + 1;
				r.setHeight(h);
				painter->fillRect(r, vacant_color);
			}
			if(vacant_after) {
				QRect r = option.rect;
				int h = r.height() / vacant_h_ratio + 1;
				r.setY(r.bottom() - h);
				//r.setHeight(h);
				painter->fillRect(r, vacant_color);
			}
		}
	}
}
