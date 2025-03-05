#include "qxclientservicewidget.h"
#include "ui_qxclientservicewidget.h"
#include "qxclientservice.h"

#include <plugins/Event/src/eventplugin.h>

#include <qf/qmlwidgets/framework/mainwindow.h>
#include <qf/qmlwidgets/dialogs/messagebox.h>
#include <qf/core/assert.h>

#include <QFileDialog>
#include <QUrlQuery>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

using qf::qmlwidgets::framework::getPlugin;

namespace Event::services::qx {

QxClientServiceWidget::QxClientServiceWidget(QWidget *parent)
	: Super(parent)
	, ui(new Ui::QxClientServiceWidget)
{
	setPersistentSettingsId("QxClientServiceWidget");
	ui->setupUi(this);
	connect(ui->edServerUrl, &QLineEdit::textChanged, this, &QxClientServiceWidget::updateOCheckListPostUrl);

	auto *svc = service();
	Q_ASSERT(svc);
	auto settings = svc->settings();
	ui->edServerUrl->setText(settings.exchangeServerUrl());
	ui->edApiToken->setText(settings.apiToken());
	auto current_stage = getPlugin<EventPlugin>()->currentStageId();
	ui->edCurrentStage->setValue(current_stage);
	connect(ui->btTestConnection, &QAbstractButton::clicked, this, &QxClientServiceWidget::testConnection);
	// connect(ui->btExportStartList, &QAbstractButton::clicked, this, [this, settings]() {
	// 	auto *manager = new QNetworkAccessManager(this);
	// 	connect(manager, &QNetworkAccessManager::finished, this, [this, manager](QNetworkReply *reply) {
	// 		if (reply->error() == QNetworkReply::NoError) {
	// 			auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	// 			if (code == 200 || code == 201) {
	// 				// exists or created
	// 				auto event_id = QString::fromUtf8(reply->readAll());
	// 				ui->edEventId->setText(event_id);
	// 				setMessage();
	// 			}
	// 		}
	// 		else {
	// 			qfError() << reply->errorString();
	// 			setMessage(reply->errorString(), true);
	// 		}
	// 		reply->deleteLater();
	// 		// One QNetworkAccessManager instance should be enough for the whole Qt application.
	// 		manager->deleteLater();
	// 	});
	// 	auto url = QUrl(settings.exchangeServerUrl() + "/api/event");
	// 	QNetworkRequest rq(url);
	// 	rq.setRawHeader("eventKey", settings.eventKey().toUtf8());
	// 	QVariantMap data = {
	// 		{"name", "Zavody"},
	// 		{"date", QDateTime::currentDateTime().toString(Qt::ISODate)},
	// 		{"place", "Praha"},
	// 	};
	// 	manager->post(rq, QJsonDocument::fromVariant(data).toJson());
	// });
}

QxClientServiceWidget::~QxClientServiceWidget()
{
	delete ui;
}

void QxClientServiceWidget::setMessage(const QString &msg, bool is_error)
{
	if (msg.isEmpty()) {
		ui->lblStatus->setStyleSheet({});
	}
	else {
		if (is_error) {
			ui->lblStatus->setStyleSheet("background: salmon");
		}
		else {
			ui->lblStatus->setStyleSheet("background: lightgreen");
		}
	}
	ui->lblStatus->setText(msg);
}

bool QxClientServiceWidget::acceptDialogDone(int result)
{
	if(result == QDialog::Accepted) {
		if(!saveSettings()) {
			return false;
		}
	}
	return true;
}

QxClientService *QxClientServiceWidget::service()
{
	auto *svc = qobject_cast<QxClientService*>(Service::serviceByName(QxClientService::serviceId()));
	QF_ASSERT(svc, QxClientService::serviceId() + " doesn't exist", return nullptr);
	return svc;
}

bool QxClientServiceWidget::saveSettings()
{
	auto *svc = service();
	if(svc) {
		auto ss = svc->settings();
		ss.setExchangeServerUrl(ui->edServerUrl->text());
		ss.setApiToken(ui->edApiToken->text());
		svc->setSettings(ss);
	}
	return true;
}

void QxClientServiceWidget::updateOCheckListPostUrl()
{
	auto url = QStringLiteral("%1/api/event/%2/ochecklist")
			.arg(ui->edServerUrl->text())
			.arg(ui->edEventId->text());
	ui->edOChecklistUrl->setText(url);
}

void QxClientServiceWidget::testConnection()
{
	auto *svc = service();
	Q_ASSERT(svc);
	auto ss = svc->settings();
	ss.setExchangeServerUrl(ui->edServerUrl->text());
	ss.setApiToken(ui->edApiToken->text());
	auto *reply = svc->loadEventInfo(ss);
	connect(reply, &QNetworkReply::finished, this, [this, reply, ss]() {
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto data = reply->readAll();
			auto doc = QJsonDocument::fromJson(data);
			auto event_info = doc.toVariant().toMap();
			ui->edEventId->setValue(event_info.value("id").toInt());
			auto remote_stage = event_info.value("stage").toInt();
			auto current_stage = getPlugin<EventPlugin>()->currentStageId();
			if (current_stage == remote_stage) {
				setMessage(tr("Connected OK"));
			}
			else {
				setMessage(tr("Connection established, but stage number must match, current stage: %1, remote stage:").arg(current_stage).arg(remote_stage), true);
			}
		}
		else {
			setMessage(tr("Connection error: %1").arg(reply->errorString()), true);
		}
	});
	connect(reply, &QNetworkReply::destroyed, []() {
		qfInfo() << "DESTROYIIIIIIIIIIIIIIIIII";
	});
}

}

