#include "sqldatadocument.h"

#include "../sql/sqlquerybuilder.h"

#include <qf/qmlwidgets/model/sqltablemodel.h>

#include <QQmlEngine>

namespace qfm = qf::qmlwidgets::model;
using namespace qf::core::qml;

SqlDataDocument::SqlDataDocument(QObject *parent)
	: Super(parent)
{

}

SqlDataDocument::~SqlDataDocument()
= default;

bool SqlDataDocument::load()
{
	qfm::SqlTableModel *m = sqlModel();
	if(m && m_qmlQueryBuilder) {
		m->setQueryBuilder(m_qmlQueryBuilder->coreBuilder(), false);
	}
	return Super::load();
}

SqlQueryBuilder *SqlDataDocument::qmlSqlQueryBuilder()
{
	if(!m_qmlQueryBuilder) {
		m_qmlQueryBuilder = new SqlQueryBuilder(this);
		QQmlEngine::setObjectOwnership(m_qmlQueryBuilder, QQmlEngine::CppOwnership);
	}
	return m_qmlQueryBuilder;
}

