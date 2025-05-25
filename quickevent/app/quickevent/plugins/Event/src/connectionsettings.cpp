#include "connectionsettings.h"

#include <plugins/Core/src/coreplugin.h>

#include <qf/core/log.h>

#include <QStandardPaths>

namespace {
const auto EVENT = QStringLiteral("event");
const auto DATA_STORAGE = QStringLiteral("dataStorage");
const auto CONNECTION_TYPE = QStringLiteral("connectionType");
const auto SQL_SERVER = QStringLiteral("sqlServer");
const auto SINGLE_FILE = QStringLiteral("singleFile");
const auto HOST = QStringLiteral("host");
const auto PORT = QStringLiteral("port");
const auto USER = QStringLiteral("user");
const auto PASSWORD = QStringLiteral("password");
const auto WORKING_DIR = QStringLiteral("workingDir");
const auto EVENT_NAME = QStringLiteral("eventName");
}

ConnectionSettings::ConnectionSettings(QObject *parent)
	: Super(parent)
{

}

void ConnectionSettings::setConnectionType(const Event::EventPlugin::ConnectionType &t)
{
	int i = static_cast<int>(t);
	setValue(EVENT + '/' + DATA_STORAGE + '/' + CONNECTION_TYPE, i);
}

void ConnectionSettings::setEventName(const QString &s)
{
	setValue(EVENT + '/' + EVENT_NAME, s);
}

void ConnectionSettings::setServerHost(const QString &s)
{
	setValue(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + CONNECTION_TYPE, s);
}

void ConnectionSettings::setServerPort(int p)
{
	setValue(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + PORT, p);
}

void ConnectionSettings::setServerUser(const QString &s)
{
	setValue(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + USER, s);
}

void ConnectionSettings::setServerPassword(const QString &s)
{
	QByteArray ba = Core::CorePlugin::encrypt(s.toUtf8(), 32);
	setValue(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + PASSWORD, QString::fromLatin1(ba));
}

void ConnectionSettings::setSingleWorkingDir(const QString &s)
{
	setValue(EVENT + '/' + DATA_STORAGE + '/' + SINGLE_FILE + '/' + WORKING_DIR, s);
}

Event::EventPlugin::ConnectionType ConnectionSettings::connectionType()
{
	int i = value(EVENT + '/' + DATA_STORAGE + '/' + CONNECTION_TYPE).toInt();
	auto t = static_cast<Event::EventPlugin::ConnectionType>(i);
	return t;
}

QString ConnectionSettings::eventName()
{
	return value(EVENT + '/' + EVENT_NAME).toString();
}

QString ConnectionSettings::serverHost()
{
	return value(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + CONNECTION_TYPE).toString();
}

int ConnectionSettings::serverPort()
{
	return value(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + PORT).toInt();
}

QString ConnectionSettings::serverUser()
{
	return value(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + USER).toString();

}

QString ConnectionSettings::serverPassword()
{
	QByteArray ba = value(EVENT + '/' + DATA_STORAGE + '/' + SQL_SERVER + '/' + PASSWORD).toString().toLatin1();
	return QString::fromUtf8(Core::CorePlugin::decrypt(ba));
}

QString ConnectionSettings::singleWorkingDir()
{
	QString dir = value(EVENT + '/' + DATA_STORAGE + '/' + SINGLE_FILE + '/' + WORKING_DIR).toString();
	if(dir.isEmpty()) {
		dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		qfInfo() << "data loc:" << dir;
	}
	return dir;
}

