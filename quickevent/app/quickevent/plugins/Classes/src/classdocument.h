#ifndef CLASSES_CLASSDOCUMENT_H
#define CLASSES_CLASSDOCUMENT_H

#include <qf/qmlwidgets/model/sqldatadocument.h>

namespace Classes {

class ClassDocument : public qf::qmlwidgets::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::model::SqlDataDocument Super;
public:
	ClassDocument(QObject *parent = nullptr);
protected:
	bool saveData() Q_DECL_OVERRIDE;
	bool dropData() Q_DECL_OVERRIDE;
};

}

#endif // CLASSDOCUMENT_H
