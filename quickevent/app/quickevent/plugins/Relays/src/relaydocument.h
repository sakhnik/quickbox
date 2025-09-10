#ifndef RELAYS_RELAYDOCUMENT_H
#define RELAYS_RELAYDOCUMENT_H

#include <qf/gui/model/sqldatadocument.h>

namespace Relays {

class RelayDocument : public qf::gui::model::SqlDataDocument
{
	Q_OBJECT
private:
	typedef qf::gui::model::SqlDataDocument Super;
public:
	 RelayDocument(QObject *parent = nullptr);
protected:
	bool saveData() Q_DECL_OVERRIDE;
	bool dropData() Q_DECL_OVERRIDE;
};

}

#endif // RELAYS_RELAYDOCUMENT_H
