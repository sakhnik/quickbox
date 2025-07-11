#include "runchangedialog.h"
#include "ui_runchangedialog.h"

#include "qxclientservice.h"
#include "runchange.h"

#include <plugins/Competitors/src/competitordocument.h>

#include <qf/core/sql/query.h>
#include <qf/core/log.h>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>

namespace Event::services::qx {

RunChangeDialog::RunChangeDialog(int change_id, int run_id, int lock_number, const RunChange &run_change, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::RunChangeDialog)
	, m_changeId(change_id)
	, m_runId(run_id)
{
	ui->setupUi(this);

	ui->btAccept->setDisabled(true);
	ui->btReject->setDisabled(true);

	ui->edClassName->setText(run_change.class_name.value_or(QString()));
	ui->edNote->setText(run_change.note);

	ui->edRunId->setValue(run_id);
	ui->edLockNumber->setValue(lock_number);

	ui->grpFirstName->setChecked(run_change.first_name.has_value());
	ui->edFirstName->setText(run_change.first_name.has_value()? run_change.first_name.value(): QString());

	ui->grpLastName->setChecked(run_change.last_name.has_value());
	ui->edLastName->setText(run_change.last_name.has_value()? run_change.last_name.value(): QString());

	ui->grpRegistration->setChecked(run_change.registration.has_value());
	ui->edRegistration->setText(run_change.registration.has_value()? run_change.registration.value(): QString());

	ui->grpSiCard->setChecked(run_change.si_id.has_value());
	ui->edSiCard->setValue(run_change.si_id.has_value()? run_change.si_id.value(): 0);

	if (run_id > 0) {
		loadOrigValues();
	}
	lockChange();
}

RunChangeDialog::~RunChangeDialog()
{
	delete ui;
}

QxClientService *RunChangeDialog::service()
{
	auto *svc = qobject_cast<QxClientService*>(Service::serviceByName(QxClientService::serviceId()));
	Q_ASSERT(svc);
	return svc;
}

void RunChangeDialog::setMessage(const QString &msg, bool error)
{
	if (msg.isEmpty()) {
		ui->lblError->setStyleSheet({});
	}
	else if (error) {
		ui->lblError->setStyleSheet("background: salmon");
	}
	else {
		ui->lblError->setStyleSheet({});
	}
	ui->lblError->setText(msg);
}

void RunChangeDialog::loadOrigValues()
{
	Q_ASSERT(m_runId > 0);

	qf::core::sql::Query q;
	q.execThrow(QStringLiteral("SELECT classes.name AS class_name, competitors.id AS competitor_id"
							   " FROM runs"
							   " JOIN competitors ON competitors.id=runs.competitorId AND runs.id=%1"
							   " LEFT JOIN classes ON competitors.classId=classes.id")
				.arg(m_runId)
				);
	int competitor_id = 0;
	if (q.next()) {
		ui->edClassName->setText(q.value("class_name").toString());
		competitor_id = q.value("competitor_id").toInt();
	}

	using namespace  qf::core::model;

	Competitors::CompetitorDocument doc;
	doc.load(competitor_id, DataDocument::ModeView);

	ui->edFirstNameOrig->setText(doc.value("firstName").toString());
	ui->edLastNameOrig->setText(doc.value("lastName").toString());
	ui->edRegistrationOrig->setText(doc.value("registration").toString());
	ui->edSiCardOrig->setValue(doc.value("siId").toInt());
}

void RunChangeDialog::lockChange()
{
	auto *svc = service();
	auto *nm = svc->networkManager();
	QNetworkRequest request;
	auto url = svc->exchangeServerUrl();
	// qfInfo() << "url " << url.toString();
	url.setPath("/api/event/current/changes/lock-change");
	QUrlQuery query;
	query.addQueryItem("change_id", QString::number(m_changeId));
	auto connection_id = QxClientService::currentConnectionId();
	query.addQueryItem("lock_number", QString::number(connection_id));
	url.setQuery(query);
	// qfInfo() << "GET " << url.toString();
	request.setUrl(url);
	request.setRawHeader(QxClientService::QX_API_TOKEN, svc->apiToken());
	auto *reply = nm->get(request);
	connect(reply, &QNetworkReply::finished, this, [this, reply, connection_id]() {
		auto data = reply->readAll();
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto id = data.toInt();
			qfInfo() << "lock reply:" << QString::fromUtf8(data) << "id:" << id;
			if (id == connection_id) {
				ui->btAccept->setDisabled(false);
				ui->btReject->setDisabled(false);

				ui->edLockNumber->setValue(id);
			}
			else {
				setMessage(tr("Change is locked already by other client: %1, current client id:.%2").arg(id).arg(connection_id), false);
			}
		}
		else {
			setMessage(tr("Lock change error: %1\n%2").arg(reply->errorString()).arg(QString::fromUtf8(data)));
		}
		reply->deleteLater();
	});
}

} // namespace Event::services::qx


