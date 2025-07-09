#pragma once

#include "../service.h"

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

namespace Event::services::qx {

static constexpr auto COL_ID = "id";
static constexpr auto COL_DATA = "data";
static constexpr auto COL_DATA_TYPE = "data_type";
static constexpr auto COL_STATUS = "status";
static constexpr auto COL_STATUS_MESSAGE = "status_message";
static constexpr auto COL_SOURCE = "source";
static constexpr auto COL_RUN_ID = "run_id";
static constexpr auto COL_USER_ID = "user_id";
static constexpr auto COL_CREATED = "created";
static constexpr auto COL_CROW_LOCK = "row_lock";

static constexpr auto STATUS_PENDING = "Pending";

class QxClientServiceSettings : public ServiceSettings
{
	using Super = ServiceSettings;

	QF_VARIANTMAP_FIELD2(QString, e, setE, xchangeServerUrl, "http://localhost:8000")
public:
	QxClientServiceSettings(const QVariantMap &o = QVariantMap()) : Super(o) {}
};

class EventInfo : public QVariantMap
{
private:
	typedef QVariantMap Super;

	QF_VARIANTMAP_FIELD(int, i, set_i, d)
	QF_VARIANTMAP_FIELD(int, s, set_s, tage)
	QF_VARIANTMAP_FIELD(int, s, set_s, tage_count)
	QF_VARIANTMAP_FIELD(QString, n, set_n, ame)
	QF_VARIANTMAP_FIELD(QString, p, set_p, lace)
	QF_VARIANTMAP_FIELD(QString, s, set_s, tart_time)
	QF_VARIANTMAP_FIELD(QVariantList, c, set_c, lasses)
public:
	EventInfo(const QVariantMap &data = QVariantMap()) : QVariantMap(data) {}
};

class QxClientService : public Service
{
	Q_OBJECT

	using Super = Service;
public:
	static constexpr auto QX_API_TOKEN = "qx-api-token";
public:
	QxClientService(QObject *parent);

	static QString serviceId();
	QString serviceDisplayName() const override;

	void run() override;
	void stop() override;
	QxClientServiceSettings settings() const {return QxClientServiceSettings(m_settings);}

	void onDbEventNotify(const QString &domain, int connection_id, const QVariant &data);
	QNetworkAccessManager* networkManager();

	QNetworkReply* getRemoteEventInfo(const QString &qxhttp_host, const QString &api_token);
	QNetworkReply* postEventInfo(const QString &qxhttp_host, const QString &api_token);

	void postStartListIofXml3(QObject *context, std::function<void (QString)> call_back = nullptr);
	void postRuns(QObject *context, std::function<void (QString)> call_back = nullptr);

	QNetworkReply* loadQxChanges(int from_id);

	QByteArray apiToken() const;
private:
	int eventId() const;
	void loadSettings() override;
	qf::qmlwidgets::framework::DialogWidget *createDetailWidget() override;
	QUrl exchangeServerUrl() const;
	void postFileCompressed(std::optional<QString> path, std::optional<QString> name, QByteArray data, QObject *context, std::function<void(QString error)> call_back = nullptr);
	enum class SpecFile {StartListIofXml3, RunsCsv};
	void uploadSpecFile(SpecFile file, QByteArray data, QObject *context, std::function<void(QString error)> call_back = nullptr);
	QByteArray zlibCompress(QByteArray data);

	void connectToSSE(int event_id);
	void disconnectSSE();

	void pollQxChanges();

	EventInfo eventInfo() const;
private:
	QNetworkAccessManager *m_networkManager = nullptr;
	QNetworkReply *m_replySSE = nullptr;
	int m_eventId = 0;
	QTimer *m_pollChangesTimer = nullptr;
};

}
