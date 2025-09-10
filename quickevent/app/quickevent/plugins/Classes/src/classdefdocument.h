#ifndef CLASSDEFDOCUMENT_H
#define CLASSDEFDOCUMENT_H

#include <qf/gui/model/sqldatadocument.h>

class ClassDefDocument : public qf::gui::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::gui::model::SqlDataDocument Super;
public:
	ClassDefDocument(QObject *parent = nullptr);
};

#endif // CLASSDEFDOCUMENT_H
