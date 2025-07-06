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
	connect(ui->edApiToken, &QLineEdit::textChanged, this, &QxClientServiceWidget::updateOCheckListPostUrl);

	setMessage("");

	auto *svc = service();
	Q_ASSERT(svc);
	auto *event_plugin = getPlugin<EventPlugin>();
	auto current_stage = event_plugin->currentStageId();
	auto settings = svc->settings();
	ui->edServerUrl->setText(settings.exchangeServerUrl());
	ui->edApiToken->setText(svc->apiToken());
	ui->edCurrentStage->setValue(current_stage);
	connect(ui->btTestConnection, &QAbstractButton::clicked, this, &QxClientServiceWidget::testConnection);
	connect(ui->btExportEventInfo, &QAbstractButton::clicked, this, &QxClientServiceWidget::exportEventInfo);
	connect(ui->btExportStartList, &QAbstractButton::clicked, this, &QxClientServiceWidget::exportStartList);
	connect(ui->btExportRuns, &QAbstractButton::clicked, this, &QxClientServiceWidget::exportRuns);
}

QxClientServiceWidget::~QxClientServiceWidget()
{
	delete ui;
}

void QxClientServiceWidget::setMessage(const QString &msg, MessageType msg_type)
{
	if (msg.isEmpty()) {
		ui->lblStatus->setStyleSheet({});
	}
	else {
		switch (msg_type) {
		case MessageType::Ok:
		ui->lblStatus->setStyleSheet("background: lightgreen");
		break;
		case MessageType::Error:
		ui->lblStatus->setStyleSheet("background: salmon");
		break;
		case MessageType::Progress:
		ui->lblStatus->setStyleSheet("background: orange");
		break;
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
		svc->setSettings(ss);
		auto *event_plugin = getPlugin<EventPlugin>();

		auto current_stage = event_plugin->currentStageId();
		auto stage_data = event_plugin->stageData(current_stage);
		stage_data.setQxApiToken(ui->edApiToken->text());
		event_plugin->setStageData(current_stage, stage_data);
	}
	return true;
}

void QxClientServiceWidget::updateOCheckListPostUrl()
{
	auto url = QStringLiteral("%1/api/event/current/oc").arg(ui->edServerUrl->text());
	ui->edOChecklistUrl->setText(url);
	ui->edOChecklistUrlHeader->setText(QStringLiteral("qx-api-token=%1").arg(ui->edApiToken->text()));
}

void QxClientServiceWidget::testConnection()
{
	auto *svc = service();
	Q_ASSERT(svc);
	auto *reply = svc->getRemoteEventInfo(ui->edServerUrl->text(), ui->edApiToken->text());
	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto data = reply->readAll();
			auto doc = QJsonDocument::fromJson(data);
			EventInfo event_info(doc.toVariant().toMap());
			ui->edEventId->setValue(event_info.id());
			setMessage(tr("Connected OK"));
		}
		else {
			setMessage(tr("Connection error: %1").arg(reply->errorString()), MessageType::Error);
		}
	});
}

void QxClientServiceWidget::exportEventInfo()
{
	auto *svc = service();
	Q_ASSERT(svc);
	auto *reply = svc->postEventInfo(ui->edServerUrl->text(), ui->edApiToken->text());
	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		auto data = reply->readAll();
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto doc = QJsonDocument::fromJson(data);
			EventInfo event_info(doc.toVariant().toMap());
			ui->edEventId->setValue(event_info.id());
			setMessage(tr("Event info updated OK"));
		}
		else {
			setMessage(tr("Event info update error: %1\n%2").arg(reply->errorString()).arg(QString::fromUtf8(data)), MessageType::Error);
		}
	});
}

void QxClientServiceWidget::exportStartList()
{
	auto *svc = service();
	Q_ASSERT(svc);
	saveSettings();
	setMessage(tr("Start list export started ..."), MessageType::Progress);
	svc->exportStartListIofXml3(this, [this](auto err) {
		if (err.isEmpty()) {
			setMessage(tr("Start list exported Ok"));
		}
		else {
			setMessage(err, MessageType::Error);
		}
	});
}

void QxClientServiceWidget::exportRuns()
{
	auto *svc = service();
	Q_ASSERT(svc);
	saveSettings();
	setMessage(tr("Runs export started ..."), MessageType::Progress);
	svc->exportRuns(this, [this](auto err) {
		if (err.isEmpty()) {
			setMessage(tr("Runs exported Ok"));
		}
		else {
			setMessage(err, MessageType::Error);
		}
	});
}

}

