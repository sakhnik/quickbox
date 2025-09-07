#pragma once

#include <qf/gui/framework/partwidget.h>

class PartWidget : public qf::gui::framework::PartWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::PartWidget Super;
public:
	explicit PartWidget(const QString &title, const QString &feature_id, QWidget *parent = nullptr);

	Q_SIGNAL void resetPartRequest();
	Q_SIGNAL void reloadPartRequest();
protected:
	Q_SLOT virtual void onActiveChanged();
};


