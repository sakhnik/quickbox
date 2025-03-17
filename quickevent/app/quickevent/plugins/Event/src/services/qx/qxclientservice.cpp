#include "qxclientservice.h"
#include "qxclientservicewidget.h"

#include "../../eventplugin.h"
#include "../../../../Runs/src/runsplugin.h"

#include <qf/qmlwidgets/framework/mainwindow.h>
#include <qf/core/log.h>
#include <qf/core/sql/query.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QJsonDocument>
#include <QUrlQuery>

using namespace qf::core;
using namespace qf::qmlwidgets;
using namespace qf::qmlwidgets::dialogs;
using namespace qf::core::sql;
using qf::qmlwidgets::framework::getPlugin;
using Event::EventPlugin;
using Runs::RunsPlugin;

namespace Event::services::qx {
//===============================================
// QxClientServiceSettings
//===============================================
// QString QxClientServiceSettings::eventKey() const
// {
// 	auto *event_plugin = getPlugin<EventPlugin>();
// 	auto *cfg = event_plugin->eventConfig();
// 	auto key = cfg->apiKey();
// 	auto current_stage = cfg->currentStageId();
// 	return QStringLiteral("%1%2").arg(key).arg(current_stage);
// }

//===============================================
// QxClientService
//===============================================
QxClientService::QxClientService(QObject *parent)
	: Super(QxClientService::serviceId(), parent)
{
	auto *event_plugin = getPlugin<EventPlugin>();

	connect(event_plugin, &EventPlugin::eventOpenChanged, this, [](bool is_open) {
		if (is_open) {
		}
		else {
		}
	});

	connect(event_plugin, &Event::EventPlugin::dbEventNotify, this, &QxClientService::onDbEventNotify, Qt::QueuedConnection);
}

QString QxClientService::serviceDisplayName() const
{
	return tr("QE Exchange");
}

QString QxClientService::serviceId()
{
	return QStringLiteral("qx");
}

void QxClientService::run() {
	auto ss = settings();
	auto *reply = getRemoteEventInfo(ss.exchangeServerUrl(), apiToken());
	connect(reply, &QNetworkReply::finished, this, [this, reply, ss]() {
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto data = reply->readAll();
			auto doc = QJsonDocument::fromJson(data);
			EventInfo event_info(doc.toVariant().toMap());
			setStatusMessage(event_info.name());
			m_eventId = event_info.id();
			connectToSSE(m_eventId);
			Super::run();
		}
		else {
			qfWarning() << "Cannot run QX service, network error:" << reply->errorString();
		}
	});
}

void QxClientService::stop()
{
	disconnectSSE();
	Super::stop();
}

qf::qmlwidgets::framework::DialogWidget *QxClientService::createDetailWidget()
{
	auto *w = new QxClientServiceWidget();
	return w;
}

void QxClientService::loadSettings()
{
	Super::loadSettings();
	auto ss = settings();
	if (ss.exchangeServerUrl().isEmpty()) {
		ss.setExchangeServerUrl("http://localhost:8000");
	}
	m_settings = ss;
}

void QxClientService::onDbEventNotify(const QString &domain, int connection_id, const QVariant &data)
{
	if (status() != Status::Running) {
		return;
	}
	Q_UNUSED(connection_id)
	Q_UNUSED(data)
	if(domain == QLatin1String(Event::EventPlugin::DBEVENT_CARD_PROCESSED_AND_ASSIGNED)) {
		//auto checked_card = quickevent::core::si::CheckedCard(data.toMap());
		//int competitor_id = getPlugin<RunsPlugin>()->competitorForRun(checked_card.runId());
		//onCompetitorChanged(competitor_id);
	}
	else if(domain == QLatin1String(Event::EventPlugin::DBEVENT_COMPETITOR_EDITED)) {
		//int competitor_id = data.toInt();
		//onCompetitorChanged(competitor_id);
	}
	else if (domain == Event::EventPlugin::DBEVENT_RUN_CHANGED) {
		//if (auto *node = m_rootNode->findChild<CurrentStageRunsNode*>(); node) {
		//	node->sendRunChangedSignal(data);
		//}
	}
}

QNetworkAccessManager *QxClientService::networkManager()
{
	if (!m_networkManager) {
		m_networkManager = new QNetworkAccessManager(this);
	}
	return m_networkManager;
}

QNetworkReply *QxClientService::getRemoteEventInfo(const QString &qxhttp_host, const QString &api_token)
{
	auto *nm = networkManager();
	QNetworkRequest request;
	QUrl url(qxhttp_host);
	url.setPath("/api/event/current");
	request.setUrl(url);
	request.setRawHeader("qx-api-token", api_token.toUtf8());
	return nm->get(request);
}

QNetworkReply *QxClientService::postEventInfo(const QString &qxhttp_host, const QString &api_token)
{
	auto *nm = networkManager();
	QNetworkRequest request;
	QUrl url(qxhttp_host);
	// qfInfo() << "url " << url.toString();
	url.setPath("/api/event/current");
	// qfInfo() << "GET " << url.toString();
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("qx-api-token", api_token.toUtf8());
	auto *event_plugin = getPlugin<EventPlugin>();
	auto *event_config = event_plugin->eventConfig();
	EventInfo ei;
	ei.set_stage(event_plugin->currentStageId());
	ei.set_name(event_config->eventName());
	ei.set_place(event_config->eventPlace());
	ei.set_start_time(event_plugin->stageStartDateTime(event_plugin->currentStageId()).toString(Qt::ISODate));
	auto data = QJsonDocument::fromVariant(ei).toJson();
	return nm->post(request, data);
}

void QxClientService::exportStartListIofXml3(QObject *context, std::function<void (QString)> call_back)
{
	auto *ep = getPlugin<EventPlugin>();
	int current_stage = ep->currentStageId();
	bool is_relays = ep->eventConfig()->isRelays();
	if (!is_relays) {
		auto xml = getPlugin<RunsPlugin>()->startListStageIofXml30(current_stage);
		uploadSpecFile(SpecFile::StartListIofXml3, xml.toUtf8(), context, call_back);
	}
}

QByteArray QxClientService::apiToken() const
{
	auto *event_plugin = getPlugin<EventPlugin>();
	auto current_stage = event_plugin->currentStageId();
	return event_plugin->stageData(current_stage).qxApiToken().toUtf8();
}

QUrl QxClientService::exchangeServerUrl() const
{
	auto ss = settings();
	return QUrl(ss.exchangeServerUrl());
}

void QxClientService::sendFile(std::optional<QString> path, std::optional<QString> name, QByteArray data, QObject *context , std::function<void (QString)> call_back)
{
	auto url = exchangeServerUrl();

	url.setPath(path.value_or("/api/event/current/file"));
	if (name.has_value()) {
		url.setQuery(QStringLiteral("name=%1").arg(name.value()));
	}
	QNetworkRequest request;
	request.setUrl(url);
	request.setRawHeader("qx-api-token", apiToken());
	request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/zip"));
	auto zdata = zlibCompress(data);
	QNetworkReply *reply = networkManager()->post(request, zdata);

	connect(reply, &QNetworkReply::finished, context, [reply, name, call_back]() {
		QString err;
		if(reply->error()) {
			err = reply->errorString();
			qfWarning() << "Post file:" << name.value_or("SPEC") << "error:" << err;
		}
		if (call_back) {
			call_back(err);
		}
		reply->deleteLater(); // should be called by Qt anyway
	});
}

void QxClientService::uploadSpecFile(SpecFile file, QByteArray data, QObject *context, std::function<void (QString)> call_back)
{
	switch (file) {
	case SpecFile::StartListIofXml3:
		sendFile("/api/event/current/upload/startlist", {}, data, context, call_back);
		break;
	}
}

QByteArray QxClientService::zlibCompress(QByteArray data)
{
	QByteArray compressedData = qCompress(data);
	// strip the 4-byte length put on by qCompress
	// internally qCompress uses zlib
	compressedData.remove(0, 4);
	return compressedData;
}

void QxClientService::connectToSSE(int event_id)
{
	auto url = exchangeServerUrl();
	url.setPath(QStringLiteral("/api/event/%1/run/changes/sse").arg(event_id));
	QNetworkRequest request(url);
	request.setRawHeader(QByteArray("Accept"), QByteArray("text/event-stream"));
	request.setHeader(QNetworkRequest::UserAgentHeader, "QuickEvent");
	request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
	request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork); // Events shouldn't be cached

	qfInfo() << "Connecting to SSE:" << url.toString();
	m_replySSE = networkManager()->get(request);
	qfInfo() << "Connected";
	connect(m_replySSE, &QNetworkReply::readyRead, this, [this]() {
		auto data = m_replySSE->readAll();
		qfInfo() << "DATA:" << data.toStdString();
	});
	connect(m_replySSE, &QNetworkReply::finished, this, [this]() {
		qfInfo() << "SSE finished:" << m_replySSE->errorString();
	});
}

void QxClientService::disconnectSSE()
{
	if (m_replySSE) {
		qfInfo() << "Disconnecting SSE:" << m_replySSE;
		m_replySSE->deleteLater();
		m_replySSE = nullptr;
	}
}

} // namespace Event::services::qx
