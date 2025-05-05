#include "competitorswidget.h"
#include "ui_competitorswidget.h"
#include "stationsbackupmemorywidget.h"

#include "competitorsplugin.h"

#include <plugins/Event/src/eventplugin.h>

#include <quickevent/core/si/siid.h>
#include <quickevent/core/si/punchrecord.h>

#include <qf/qmlwidgets/dialogs/dialog.h>
#include <qf/qmlwidgets/dialogs/getiteminputdialog.h>
#include <qf/qmlwidgets/dialogs/messagebox.h>
#include <qf/qmlwidgets/framework/mainwindow.h>
#include <qf/qmlwidgets/framework/plugin.h>
#include <qf/qmlwidgets/toolbar.h>
#include <qf/qmlwidgets/combobox.h>
#include <qf/qmlwidgets/action.h>
#include <qf/qmlwidgets/menubar.h>
#include <qf/qmlwidgets/dialogbuttonbox.h>
#include <qf/qmlwidgets/reports/widgets/reportviewwidget.h>
#include <qf/core/model/sqltablemodel.h>
#include <qf/core/sql/query.h>
#include <qf/core/sql/querybuilder.h>
#include <qf/core/sql/transaction.h>
#include <qf/core/assert.h>
#include <qf/core/utils/treetable.h>

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

namespace qfs = qf::core::sql;
namespace qfw = qf::qmlwidgets;
namespace qff = qf::qmlwidgets::framework;
namespace qfd = qf::qmlwidgets::dialogs;
namespace qfc = qf::core;
namespace qfm = qf::core::model;
namespace qfu = qf::core::utils;


using qf::qmlwidgets::framework::getPlugin;
using Event::EventPlugin;
using Competitors::CompetitorsPlugin;

class CompetitorsModel : public qfm::SqlTableModel
{
	using Super = qfm::SqlTableModel;
public:
	CompetitorsModel(QObject *parent ) : Super(parent) {}

	bool postRow(int row_no, bool throw_exc) override
	{
		qfu::TableRow &row_ref = m_table.rowRef(row_no);
		auto competitor_id = row_ref.value("competitors.id").toInt();
		QVariantMap dirty_rec;
		if (!row_ref.isInsert()) {
			Q_ASSERT(competitor_id > 0);
			dirty_rec = row_ref.dirtyValuesMap();
		}
		auto ret = Super::postRow(row_no, throw_exc);
		if (!dirty_rec.isEmpty()) {
			qf::core::sql::Query q;
			q.exec(QStringLiteral("SELECT id FROM runs WHERE competitorId = %1").arg(competitor_id));
			while(q.next()) {
				int run_id = q.value(0).toInt();
				getPlugin<EventPlugin>()->emitDbEvent(Event::EventPlugin::DBEVENT_RUN_CHANGED, QVariantList {run_id, dirty_rec});
			}
		}
		return ret;
	}
};

CompetitorsWidget::CompetitorsWidget(QWidget *parent) :
	Super(parent),
	ui(new Ui::CompetitorsWidget)
{
	ui->setupUi(this);

	ui->tblCompetitorsToolBar->setTableView(ui->tblCompetitors);

	ui->tblCompetitors->setCloneRowEnabled(false);
	ui->tblCompetitors->setDirtyRowsMenuSectionEnabled(false);
	ui->tblCompetitors->setPersistentSettingsId("tblCompetitors");
	ui->tblCompetitors->setRowEditorMode(qfw::TableView::EditRowsMixed);
	ui->tblCompetitors->setInlineEditSaveStrategy(qfw::TableView::OnEditedValueCommit);
	qfm::SqlTableModel *m = new CompetitorsModel(this);
	m->addColumn("id").setReadOnly(true);
	m->addColumn("classes.name", tr("Class"));
	m->addColumn("competitors.startNumber", tr("SN", "start number")).setToolTip(tr("Start number"));
	m->addColumn("competitorName", tr("Name"));
	m->addColumn("registration", tr("Reg")).setToolTip(tr("Czech registration number"));;
	m->addColumn("iofId", tr("IOF ID")).setToolTip(tr("IOF ID number"));;
	m->addColumn("siId", tr("SI")).setReadOnly(true).setCastType(qMetaTypeId<quickevent::core::si::SiId>());
	m->addColumn("ranking", tr("Ranking pos")).setToolTip(tr("Runner's position in CZ ranking."));
	m->addColumn("note", tr("Note"));
	ui->tblCompetitors->setTableModel(m);
	m_competitorsModel = m;

	ui->tblCompetitors->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tblCompetitors, &qfw::TableView::customContextMenuRequested, this, &CompetitorsWidget::onCustomContextMenuRequest);

	QMetaObject::invokeMethod(this, "lazyInit", Qt::QueuedConnection);
}

CompetitorsWidget::~CompetitorsWidget()
{
	delete ui;
}

void CompetitorsWidget::settleDownInPartWidget(::PartWidget *part_widget)
{
	connect(part_widget, &::PartWidget::resetPartRequest, this, &CompetitorsWidget::reset);
	connect(part_widget, &::PartWidget::reloadPartRequest, this, &CompetitorsWidget::reload);

	qfw::ToolBar *main_tb = part_widget->toolBar("main", true);
	{
		QLabel *lbl;
		{
			lbl = new QLabel(tr("&Class "));
			main_tb->addWidget(lbl);
		}
		{
			m_cbxClasses = new qfw::ForeignKeyComboBox();
#if QT_VERSION_MAJOR >= 6
			m_cbxClasses->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
#else
			m_cbxClasses->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
			m_cbxClasses->setMinimumWidth(fontMetrics().horizontalAdvance('X') * 15);
#else
			m_cbxClasses->setMinimumWidth(fontMetrics().width('X') * 15);
#endif
			m_cbxClasses->setMaxVisibleItems(100);
			m_cbxClasses->setReferencedTable("classes");
			m_cbxClasses->setReferencedField("id");
			m_cbxClasses->setReferencedCaptionField("name");
			main_tb->addWidget(m_cbxClasses);
		}
		lbl->setBuddy(m_cbxClasses);
	}

	qf::qmlwidgets::Action *act_stations = part_widget->menuBar()->actionForPath("stations");
	act_stations->setText(tr("&Stations"));
	{
		qf::qmlwidgets::Action *a = new qf::qmlwidgets::Action("backupMemory", tr("Backup memory"));
		act_stations->addActionInto(a);
		connect(a, &qf::qmlwidgets::Action::triggered, [this]() {
			qf::qmlwidgets::dialogs::Dialog dlg(this);
			auto *w = new StationsBackupMemoryWidget();
			dlg.setCentralWidget(w);
			dlg.exec();
		});
	}
}

void CompetitorsWidget::lazyInit()
{
}

void CompetitorsWidget::reset()
{
	if(!getPlugin<EventPlugin>()->isEventOpen()) {
		m_competitorsModel->clearRows();
		return;
	}
	{
		m_cbxClasses->blockSignals(true);
		m_cbxClasses->loadItems(true);
		m_cbxClasses->insertItem(0, tr("--- all ---"), 0);
		m_cbxClasses->setCurrentIndex(0);
		connect(m_cbxClasses, &qf::qmlwidgets::ForeignKeyComboBox::currentDataChanged, this, &CompetitorsWidget::reload, Qt::UniqueConnection);
		m_cbxClasses->blockSignals(false);
	}
	reload();
}

void CompetitorsWidget::reload()
{
	bool is_relays = getPlugin<EventPlugin>()->eventConfig()->isRelays();
	qfs::QueryBuilder qb;
	qb.select2("competitors", "*")
			.select2("classes", "name")
			.select("COALESCE(lastName, '') || ' ' || COALESCE(firstName, '') AS competitorName")
			.from("competitors")
			.orderBy("competitors.id");//.limit(10);
	int class_id = m_cbxClasses->currentData().toInt();
	if(class_id > 0) {
		qb.where("classes.id=" + QString::number(class_id));
	}
	QString join_kind = (class_id > 0)? qfs::QueryBuilder::INNER_JOIN: qfs::QueryBuilder::LEFT_JOIN;
	if(is_relays) {
		qb.join("competitors.id", "runs.competitorId", join_kind);
		qb.join("runs.relayId", "relays.id", join_kind);
		qb.join("relays.classId", "classes.id", join_kind);
	}
	else {
		qb.join("competitors.classId", "classes.id", join_kind);
	}
	m_competitorsModel->setQueryBuilder(qb, false);
	m_competitorsModel->reload();
}

void CompetitorsWidget::onCustomContextMenuRequest(const QPoint &pos)
{
	qfLogFuncFrame();
	QAction a_change_class(tr("Set class in selected rows"), nullptr);
	QList<QAction*> lst;
	lst << &a_change_class;
	QAction *a = QMenu::exec(lst, ui->tblCompetitors->viewport()->mapToGlobal(pos));
	if(a == &a_change_class) {
		qfw::dialogs::GetItemInputDialog dlg(this);
		QComboBox *box = dlg.comboBox();
		qfs::Query q;
		q.exec("SELECT id, name FROM classes ORDER BY name");
		while (q.next()) {
			box->addItem(q.value(1).toString(), q.value(0));
		}
		dlg.setWindowTitle(tr("Dialog"));
		dlg.setLabelText(tr("Select class"));
		dlg.setCurrentItemIndex(-1);
		if(dlg.exec()) {
			int class_id = dlg.currentData().toInt();
			if(class_id > 0) {
				qfs::Transaction transaction;
				try {
					QList<int> rows = ui->tblCompetitors->selectedRowsIndexes();
					for(int i : rows) {
						qf::core::utils::TableRow row = ui->tblCompetitors->tableRowRef(i);
						int competitor_id = row.value("competitors.id").toInt();
						q.exec(QString("UPDATE competitors SET classId=%1 WHERE id=%2").arg(class_id).arg(competitor_id), qfc::Exception::Throw);
					}
					transaction.commit();
				}
				catch (std::exception &e) {
					qfError() << e.what();
				}
			}
			ui->tblCompetitors->reload(true);
		}
	}
}

