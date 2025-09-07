#ifndef QF_GUI_HTMLVIEWWIDGET_H
#define QF_GUI_HTMLVIEWWIDGET_H

#include "guiglobal.h"
#include "framework/dialogwidget.h"
#include "framework/ipersistentoptions.h"

namespace qf {
namespace gui {

namespace Ui {
class HtmlViewWidget;
}

class QFGUI_DECL_EXPORT HtmlViewWidget : public qf::gui::framework::DialogWidget
{
	Q_OBJECT
private:
	typedef qf::gui::framework::DialogWidget Super;
public:
	explicit HtmlViewWidget(QWidget *parent = nullptr);
	~HtmlViewWidget() Q_DECL_OVERRIDE;

	//QString htmlText();
	void setHtmlText(const QString &html_text);

private:
	ActionMap createActions() Q_DECL_OVERRIDE;
	void settleDownInDialog(qf::gui::dialogs::Dialog *dlg) Q_DECL_OVERRIDE;
private:
	Ui::HtmlViewWidget *ui;
};


} // namespace gui
} // namespace qf
#endif // QF_GUI_HTMLVIEWWIDGET_H
