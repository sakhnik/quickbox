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
	Super::run();
	auto ss = settings();
	auto *reply = loadEventInfo(ss);
	connect(reply, &QNetworkReply::finished, this, [this, reply, ss]() {
		if (reply->error() == QNetworkReply::NetworkError::NoError) {
			auto data = reply->readAll();
			auto doc = QJsonDocument::fromJson(data);
			auto event_info = doc.toVariant().toMap();
			if (m_eventId = event_info.value("id").toInt(); m_eventId > 0) {
				auto current_stage = getPlugin<EventPlugin>()->currentStageId();
				if (auto remote_stage = event_info.value("stage").toInt(); remote_stage == current_stage) {
					return;
				}
				qfWarning() << "Cannot run QX service, stage numbers mismatch";
			}
		}
		else {
			qfWarning() << "Cannot run QX service, network error:" << reply->errorString();
		}
		stop();
	});
}

void QxClientService::stop() {
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

QNetworkReply *QxClientService::loadEventInfo(QxClientServiceSettings settings)
{
	auto *nm = networkManager();
	QNetworkRequest request;
	QUrl url(settings.exchangeServerUrl());
	qfInfo() << "url " << url.toString();
	url.setPath("/api/event/current");
	qfInfo() << "GET " << url.toString();
	request.setUrl(url);
	request.setRawHeader("qx-api-token", settings.apiToken().toUtf8());

	return nm->get(request);
	// connect(reply, &QNetworkReply::errorOccurred, watcher, [watcher, reply]() {
	// 	// qfWarning() << "Network reply error:" << err.toString();
	// 	watcher->setError(reply->errorString());
	// });
	// connect(reply, &QNetworkReply::sslErrors, watcher, [watcher]() {
	// 	// qfWarning() << "SSL error";
	// 	watcher->setError("SSL error");
	// });
}

} // namespace Event::services::qx
