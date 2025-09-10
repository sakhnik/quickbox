#ifndef EVENTDIALOGWIDGET_H
#define EVENTDIALOGWIDGET_H

#include <qf/gui/framework/dialogwidget.h>

namespace Ui {
class EventDialogWidget;
}

class EventDialogWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	explicit EventDialogWidget(QWidget *parent = nullptr);
	~EventDialogWidget() Q_DECL_OVERRIDE;

	void setEventId(const QString &event_id);
	QString eventId() const;
	void setEventIdEditable(bool b);

	void loadParams(const QVariantMap &params);
	QVariantMap saveParams();
private:
	Ui::EventDialogWidget *ui;
};

#endif // EVENTDIALOGWIDGET_H
