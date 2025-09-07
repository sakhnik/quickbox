#include "htmlviewwidget.h"
#include "ui_htmlviewwidget.h"


namespace qf::gui {

HtmlViewWidget::HtmlViewWidget(QWidget *parent) :
	Super(parent),
	ui(new Ui::HtmlViewWidget)
{
	setPersistentSettingsId("HtmlViewWidget");
	ui->setupUi(this);
}

HtmlViewWidget::~HtmlViewWidget()
{
	delete ui;
}

void HtmlViewWidget::setHtmlText(const QString &html_text)
{
	ui->textBrowser->setHtml(html_text);
}

framework::DialogWidget::ActionMap HtmlViewWidget::createActions()
{
	ActionMap ret = Super::createActions();
	/*
	QStyle *sty = style();
	{
		QIcon ico(":/qf/gui/images/print");
		qf::gui::Action *a = new qf::gui::Action(ico, tr("Print"), this);
		ret[QStringLiteral("print")] = a;
		connect(a, SIGNAL(triggered()), this, SLOT(print()));
	}
	{
		QIcon ico = sty->standardIcon(QStyle::SP_DialogSaveButton);
		qf::gui::Action *a = new qf::gui::Action(ico, tr("Save"), this);
		ret[QStringLiteral("save")] = a;
		connect(a, SIGNAL(triggered()), this, SLOT(save()));
	}
	{
		QIcon ico = sty->standardIcon(QStyle::SP_DialogSaveButton);
		qf::gui::Action *a = new qf::gui::Action(ico, tr("Save as"), this);
		ret[QStringLiteral("saveAs")] = a;
		connect(a, SIGNAL(triggered()), this, SLOT(saveAs()));
	}
	{
		QIcon ico(":/qf/gui/images/wordwrap");
		qf::gui::Action *a = new qf::gui::Action(ico, tr("Wrap lines"), this);
		a->setCheckable(true);
		ret[QStringLiteral("wrapLines")] = a;
		connect(a, SIGNAL(triggered(bool)), this, SLOT(wrapLines(bool)));
	}
	*/
	return ret;
}

void HtmlViewWidget::settleDownInDialog(dialogs::Dialog *dlg)
{
	/*
	qfLogFuncFrame() << dlg;
	qf::gui::Action *act_file = dlg->menuBar()->actionForPath("file");
	act_file->setText(tr("&File"));
	act_file->addActionInto(action("save"));
	act_file->addActionInto(action("saveAs"));
	act_file->addSeparatorInto();
	act_file->addActionInto(action("print"));

	qf::gui::Action *act_view = dlg->menuBar()->actionForPath("view");
	act_view->setText(tr("&View"));
	act_view->addActionInto(action("wrapLines"));

	qf::gui::ToolBar *tool_bar = dlg->toolBar("main", true);
	tool_bar->addAction(action("save"));
	tool_bar->addAction(action("print"));
	tool_bar->addAction(action("wrapLines"));
	*/
	Super::settleDownInDialog(dlg);
}

} // namespace qf::gui

