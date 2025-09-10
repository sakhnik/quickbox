
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//

#include "theapp.h"
#include "driver/qfhttpmysql/qfhttpmysql.h"

#include <qf/gui/style.h>

#include <qf/core/log.h>

#include <QSqlDriverCreatorBase>

//======================================================
//                   SqlJournal
//======================================================
void SqlJournal::log(const QString& msg)
{
	if(msg.startsWith("select ", Qt::CaseInsensitive)) return;
	if(msg.startsWith("show ", Qt::CaseInsensitive)) return;
	if(msg.startsWith("use ", Qt::CaseInsensitive)) {
		/// pokud minule use bylo stejne, neloguj ho
		if(m_log.count() && m_log.last() == msg) return;
	}
	//qfInfo() << msg;
	m_log << msg;
}

//======================================================
//                   QFHttpMySqlDriverCreator
//======================================================
class QFHttpMySqlDriverCreator : public QSqlDriverCreatorBase
{
	public:
		QSqlDriver* createObject() const override {return new QFHttpMySqlDriver();}
};

//======================================================
//                   TheApp
//======================================================
SqlJournal TheApp::f_sqlJournal;

TheApp::TheApp(int & argc, char ** argv)
	: QApplication(argc, argv)
{
	auto *style = qf::gui::Style::instance();
	//style->addIconSearchPath(":/qf/gui/images/flat");
	style->addIconSearchPath(":/qf/gui/images");

	QSqlDatabase::registerSqlDriver("QFHTTPMYSQL", new QFHttpMySqlDriverCreator());

	QStringList args = arguments();
	int ix = args.indexOf(QLatin1String("--one-time-connection-settings"));
	if(ix >= 0)
		setOneTimeConnectionSettings(args.value(ix + 1));
}

TheApp::~TheApp() = default;

TheApp* TheApp::instance()
{
	auto *a = qobject_cast<TheApp*>(QApplication::instance());
	QF_ASSERT_EX(a!=nullptr, "Application is not initialized yet");
	return a;
}

qf::core::utils::Crypt TheApp::crypt()
{
	return qf::core::utils::Crypt();
}

QString TheApp::versionString() const
{
	return QCoreApplication::applicationVersion();
}

SqlJournal * TheApp::sqlJournal()
{
	return &f_sqlJournal;
}

