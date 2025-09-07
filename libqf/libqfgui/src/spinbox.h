#ifndef QF_GUI_SPINBOX_H
#define QF_GUI_SPINBOX_H

#include "guiglobal.h"
#include "idatawidget.h"

#include <QSpinBox>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT SpinBox : public QSpinBox, public IDataWidget
{
	Q_OBJECT
	Q_PROPERTY(QVariant dataValue READ dataValue WRITE setDataValue NOTIFY dataValueChanged)
	Q_PROPERTY(QString dataId READ dataId WRITE setDataId)
private:
	typedef QSpinBox Super;
public:
	explicit SpinBox(QWidget *parent = nullptr);
	~SpinBox() Q_DECL_OVERRIDE {}
protected:
	QVariant dataValue() Q_DECL_OVERRIDE;
	void setDataValue(const QVariant &val) Q_DECL_OVERRIDE;
	Q_SIGNAL void dataValueChanged(const QVariant &new_val);
	void keyPressEvent(QKeyEvent * event) override;
private:
	Q_SLOT void onValueChanged(int n);
};

}}

#endif // QF_GUI_SPINBOX_H
