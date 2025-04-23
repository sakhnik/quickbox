#include "qxlateregistrationswidget.h"
#include "ui_qxlateregistrationswidget.h"

#include "qxlateregistrationsmodel.h"
#include "qxclientservice.h"

#include <plugins/Event/src/eventplugin.h>

namespace Event::services::qx {

QxLateRegistrationsWidget::QxLateRegistrationsWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::QxLateRegistrationsWidget)
{
	ui->setupUi(this);

	showMessage({});
	setEnabled(false);

	connect(ui->btReload, &QAbstractButton::clicked, this, &QxLateRegistrationsWidget::reload);
	connect(ui->btResizeColumns, &QAbstractButton::clicked, this, &QxLateRegistrationsWidget::resizeColumns);

	m_model = new QxLateRegistrationsModel(this);
	connect(m_model, &QxLateRegistrationsModel::modelLoadInfo, this, &QxLateRegistrationsWidget::showMessage);

	ui->tableView->setModel(m_model);

	auto *svc = service();
	connect(svc, &Service::statusChanged, this, [this](Service::Status new_status){
		switch (new_status) {
		case Service::Status::Unknown:
		case Service::Status::Stopped:
			setEnabled(false);
			break;
			case Service::Status::Running:
			setEnabled(true);
			reload();
			break;
		}
	});

}

QxLateRegistrationsWidget::~QxLateRegistrationsWidget()
{
	delete ui;
}

void QxLateRegistrationsWidget::onDbEventNotify(const QString &domain, const QVariant &payload)
{
	Q_UNUSED(payload)
	if(domain == QLatin1String(Event::EventPlugin::DBEVENT_CARD_PROCESSED_AND_ASSIGNED)
	   || domain == QLatin1String(Event::EventPlugin::DBEVENT_COMPETITOR_COUNTS_CHANGED)) {
		//reloadDeferred();
	}
}

void QxLateRegistrationsWidget::onVisibleChanged(bool is_visible)
{
	if (is_visible && isEnabled()) {
		reload();
	}
}

QxClientService *QxLateRegistrationsWidget::service()
{
	auto *svc = qobject_cast<QxClientService*>(Event::services::Service::serviceByName(QxClientService::serviceId()));
	Q_ASSERT(svc);
	return svc;
}

void QxLateRegistrationsWidget::resizeColumns()
{
	auto *tv = ui->tableView;
	tv->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void QxLateRegistrationsWidget::showMessage(const QString &msg, bool is_error)
{
	if (msg.isEmpty()) {
		ui->lblErrorMsg->setStyleSheet({});
	}
	else {
		if (is_error) {
			ui->lblErrorMsg->setStyleSheet("background: salmon");
		}
		else {
			ui->lblErrorMsg->setStyleSheet("background: lightgreen");
		}
	}
	ui->lblErrorMsg->setText(msg);
}

void QxLateRegistrationsWidget::reload()
{
	m_model->reload();
}

}
