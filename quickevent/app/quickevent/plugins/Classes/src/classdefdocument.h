#ifndef CLASSDEFDOCUMENT_H
#define CLASSDEFDOCUMENT_H

#include <qf/qmlwidgets/model/sqldatadocument.h>

class ClassDefDocument : public qf::qmlwidgets::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::model::SqlDataDocument Super;
public:
	ClassDefDocument(QObject *parent = nullptr);
};

#endif // CLASSDEFDOCUMENT_H
