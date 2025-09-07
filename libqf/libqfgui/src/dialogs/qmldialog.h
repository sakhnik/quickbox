#ifndef QF_GUI_DIALOGS_QMLDIALOG_H
#define QF_GUI_DIALOGS_QMLDIALOG_H

#include "../guiglobal.h"
#include "dialog.h"
#include "../framework/ipersistentsettings.h"
#include "../framework/dialogwidget.h"

#include <QQmlListProperty>
#include <QQmlParserStatus>

namespace qf {
namespace gui {

class Frame;

namespace dialogs {

class QFGUI_DECL_EXPORT QmlDialog : public Dialog, public QQmlParserStatus
{
	Q_OBJECT
	Q_INTERFACES(QQmlParserStatus)
	Q_ENUM(DoneResult)
	Q_ENUM(QDialogButtonBox::StandardButton)
	Q_CLASSINFO("DefaultProperty", "widgets")
	Q_PROPERTY(QQmlListProperty<QWidget> widgets READ widgets)
	Q_PROPERTY(qf::gui::framework::DialogWidget* dialogWidget READ dialogWidget WRITE setDialogWidget NOTIFY dialogWidgetChanged)
	Q_PROPERTY(int standardButtons READ standardButtons WRITE setStandardButtons)
private:
	typedef Dialog Super;
public:
	explicit QmlDialog(QWidget *parent = nullptr);
	~QmlDialog() Q_DECL_OVERRIDE;

	void setStandardButtons(int bts) {setButtons((QDialogButtonBox::StandardButtons)bts);}
	int standardButtons();

	qf::gui::framework::DialogWidget* dialogWidget() const;
	Q_SLOT void setDialogWidget(qf::gui::framework::DialogWidget* w);
	Q_SIGNAL void dialogWidgetChanged(framework::DialogWidget* w);
protected:
	QQmlListProperty<QWidget> widgets();
private:
	void classBegin() Q_DECL_OVERRIDE;
	void componentComplete() Q_DECL_OVERRIDE;
protected:
	Frame *m_centralFrame;
};

}}}

#endif // QF_GUI_DIALOGS_QMLDIALOG_H
