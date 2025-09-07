#ifndef EVENT_STAGEDOCUMENT_H
#define EVENT_STAGEDOCUMENT_H

#include <qf/gui/model/sqldatadocument.h>

namespace Event {

class StageDocument : public qf::gui::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::gui::model::SqlDataDocument Super;
public:
	StageDocument(QObject *parent = nullptr);
};

}

#endif // EVENT_STAGEDOCUMENT_H
