#ifndef QF_GUI_REPORTS_REPORTDOCUMENT_H
#define QF_GUI_REPORTS_REPORTDOCUMENT_H

#include <qf/core/exception.h>

#include <QQmlComponent>
#include <QCoreApplication>

namespace qf {
namespace gui {
namespace reports {

class ReportDocument : public QQmlComponent
{
private:
	typedef QQmlComponent Super;
public:
	explicit ReportDocument(QQmlEngine *engine, QObject *parent = nullptr);
public:
	void setFileName(const QString &fname);
};

}}}

#endif // QF_GUI_REPORTS_REPORTDOCUMENT_H
