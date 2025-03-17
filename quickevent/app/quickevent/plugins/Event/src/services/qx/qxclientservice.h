#pragma once

#include "../service.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace Event::services::qx {

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
	QF_VARIANTMAP_FIELD(QString, n, set_n, ame)
	QF_VARIANTMAP_FIELD(QString, p, set_p, lace)
	QF_VARIANTMAP_FIELD(QString, s, set_s, tart_time)
public:
	EventInfo(const QVariantMap &data = QVariantMap()) : QVariantMap(data) {}
};

class QxClientService : public Service
{
	Q_OBJECT

	using Super = Service;
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

	void exportStartListIofXml3(QObject *context, std::function<void (QString)> call_back = nullptr);
private:
	void loadSettings() override;
	qf::qmlwidgets::framework::DialogWidget *createDetailWidget() override;
	QByteArray apiToken() const;
	QUrl exchangeServerUrl() const;
	void sendFile(std::optional<QString> path, std::optional<QString> name, QByteArray data, QObject *context, std::function<void(QString error)> call_back = nullptr);
	enum class SpecFile {StartListIofXml3};
	void uploadSpecFile(SpecFile file, QByteArray data, QObject *context, std::function<void(QString error)> call_back = nullptr);
	QByteArray zlibCompress(QByteArray data);

	void connectToSSE(int event_id);
	void disconnectSSE();
private:
	QNetworkAccessManager *m_networkManager = nullptr;
	QNetworkReply *m_replySSE = nullptr;
	int m_eventId = 0;
};

}
