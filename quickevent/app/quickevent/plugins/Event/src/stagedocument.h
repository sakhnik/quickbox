#ifndef EVENT_STAGEDOCUMENT_H
#define EVENT_STAGEDOCUMENT_H

#include <qf/qmlwidgets/model/sqldatadocument.h>

namespace Event {

class StageDocument : public qf::qmlwidgets::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::model::SqlDataDocument Super;
public:
	StageDocument(QObject *parent = nullptr);
};

}

#endif // EVENT_STAGEDOCUMENT_H
