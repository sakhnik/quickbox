#ifndef QF_GUI_CHECKBOX_H
#define QF_GUI_CHECKBOX_H

#include "guiglobal.h"
#include "idatawidget.h"

#include <QCheckBox>
#include <QVariant>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT CheckBox : public QCheckBox, public IDataWidget
{
	Q_OBJECT
	Q_PROPERTY(QVariant dataValue READ dataValue WRITE setDataValue NOTIFY dataValueChanged)
	Q_PROPERTY(QString dataId READ dataId WRITE setDataId)
private:
	typedef QCheckBox Super;
public:
	explicit CheckBox(QWidget *parent = nullptr);
	~CheckBox() Q_DECL_OVERRIDE {}
protected:
	QVariant dataValue() Q_DECL_OVERRIDE;
	void setDataValue(const QVariant &val) Q_DECL_OVERRIDE;
	Q_SIGNAL void dataValueChanged(const QVariant &new_val);
private:
	Q_SLOT void onStateChanged(int st);
};

} // namespace gui
} // namespace qf

#endif // QF_GUI_CHECKBOX_H
