#pragma once

#include "../service.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace Event::services::qx {

class RootNode;

class QxClientServiceSettings : public ServiceSettings
{
	using Super = ServiceSettings;

	QF_VARIANTMAP_FIELD2(QString, e, setE, xchangeServerUrl, "http://localhost:8000")
public:
	QxClientServiceSettings(const QVariantMap &o = QVariantMap()) : Super(o) {}
};

// class NetworkReplyWatcher : public QObject
// {
// 	Q_OBJECT
// public:
// 	NetworkReplyWatcher() : QObject() {}

// 	void setData(const QVariant data) {
// 		emit finished(data, {});
// 	}
// 	void setError(const QString error) {
// 		emit finished({}, error);
// 	}
// 	Q_SIGNAL void finished(const QVariant data, QString error);
// };

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
private:
	void loadSettings() override;
	qf::qmlwidgets::framework::DialogWidget *createDetailWidget() override;
	QString apiToken() const;
private:
	QNetworkAccessManager *m_networkManager = nullptr;
	int m_eventId = 0;
};

}
