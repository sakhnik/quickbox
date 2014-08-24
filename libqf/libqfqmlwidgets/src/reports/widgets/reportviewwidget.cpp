
//
// Author: Frantisek Vacek <fanda.vacek@volny.cz>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//

#include "reportviewwidget.h"
#include "itemvalueeditorwidget.h"

#include <qfreportpainter.h>
#include <qfaction.h>
#include <qftoolbar.h>
#include <qfpixmapcache.h>
#include <qfstatusbar.h>
#include <qfsmtpclient.h>
#include <qffileutils.h>
#include <qfhtmlutils.h>
#include <qfhtmlviewutils.h>
#include <qfdlghtmlview.h>
#include <qfdlgexception.h>
#include <qfapplication.h>
#include <qfmessage.h>
#include <qfbuttondialog.h>
#include <qfdatatranslator.h>
#include <qfappdatatranslatorinterface.h>
#include <qfappdbconnectioninterface.h>
#include <qfcursoroverrider.h>

#include <QScrollBar>
#include <QMenu>
#include <QBoxLayout>
#include <QSpinBox>
#include <QBitmap>
#include <QTimer>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>

#include <typeinfo>

#include <qf/core/log.h>

using namespace qf::qmlwidgets::reports;

//====================================================
//                                 QFReportViewWidget::ScrollArea
//====================================================
QFReportViewWidget::ScrollArea::ScrollArea(QWidget * parent)
: QScrollArea(parent)
{
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScrollBarValueChanged(int)));
}

void QFReportViewWidget::ScrollArea::wheelEvent(QWheelEvent * ev)
{
	if(ev->orientation() == Qt::Vertical) {
		if(ev->modifiers() == Qt::ShiftModifier) {
			int delta = ev->delta();
			if (ev->orientation() == Qt::Horizontal) {
			}
			else {
				emit zoomOnWheel(delta, ev->pos());
			}
			ev->accept();
			return;
		}
		else {
			QScrollBar *sb = verticalScrollBar();
			if(!sb || !sb->isVisible()) {
				/// pokud neni scroll bar, nemuzu se spolehnout na funkci verticalScrollBarValueChanged(), protoze value je pro oba smery == 0
				//qfInfo() << e->delta();
				if(ev->delta() < 0) {
					emit showNextPage();
				}
				else {
					emit showPreviousPage();
				}
				ev->accept();
				return;
			}
		}
	}
	QScrollArea::wheelEvent(ev);
}

void QFReportViewWidget::ScrollArea::keyPressEvent(QKeyEvent* ev)
{
	if(ev->modifiers() == Qt::ShiftModifier) {
		static QCursor c;
		if(c.bitmap() == NULL) {
			QBitmap b1(":/libqfgui/images/zoomin_cursor_bitmap.png");
			QBitmap b2(":/libqfgui/images/zoomin_cursor_mask.png");
			c = QCursor(b1, b2, 18, 12);
		}
		setCursor(c);
	}
	QAbstractScrollArea::keyPressEvent(ev);
}

void QFReportViewWidget::ScrollArea::keyReleaseEvent(QKeyEvent* ev)
{
	setCursor(QCursor());
	QWidget::keyReleaseEvent(ev);
}

void QFReportViewWidget::ScrollArea::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton && ev->modifiers() == Qt::ShiftModifier) {
		f_dragMouseStartPos = ev->pos();
		f_dragViewportStartPos = -widget()->pos();
		setCursor(QCursor(Qt::ClosedHandCursor));
		//QPoint pos = ev->pos();
		//qfInfo() << "press start pos:" << f_dragStartPos.x() << f_dragStartPos.y() << "pos:" << pos.x() << pos.y();
		//QDrag *drag = new QDrag(this);
		//QMimeData *mimeData = new QMimeData;
		//mimeData->setText(commentEdit->toPlainText());
		//drag->setMimeData(mimeData);
		//drag->setPixmap(iconPixmap);

		//Qt::DropAction dropAction = drag->exec();
		ev->accept();
		return;
	}
	QScrollArea::mousePressEvent(ev);
}

void QFReportViewWidget::ScrollArea::mouseReleaseEvent(QMouseEvent* ev)
{
	setCursor(QCursor());
	QAbstractScrollArea::mouseReleaseEvent(ev);
}

void QFReportViewWidget::ScrollArea::mouseMoveEvent(QMouseEvent* ev)
{
	//qfInfo() << "move buttons:" << ev->buttons() << ev->button() << "mod:" << QString::number(ev->modifiers(), 16) << (ev->button() == Qt::LeftButton) << (ev->modifiers() == Qt::ShiftModifier);
	if(ev->buttons() == Qt::LeftButton && ev->modifiers() == Qt::ShiftModifier) {
		QPoint pos = ev->pos();
		//qfInfo() << "start pos:" << f_dragStartPos.x() << f_dragStartPos.y() << "pos:" << pos.x() << pos.y();
		/// dej startpos na pos
		QScrollBar *hsb = horizontalScrollBar();
		QScrollBar *vsb = verticalScrollBar();
		//QSize ws = widget()->size();
		//QRect wr = widget()->geometry();
		if(hsb) {
			hsb->setValue(f_dragViewportStartPos.x() - (pos.x() - f_dragMouseStartPos.x()));
			//qfTrash() << "\t horizontal scrollbar value2:" << hsb->value() << "of" << hsb->maximum();
		}
		if(vsb) {
			vsb->setValue(f_dragViewportStartPos.y() - (pos.y() - f_dragMouseStartPos.y()));
			//qfTrash() << "\t vertical scrollbar value2:" << vsb->value() << "of" << vsb->maximum();
		}
		ev->accept();
		return;
	}
	QScrollArea::mouseMoveEvent(ev);
}

void QFReportViewWidget::ScrollArea::verticalScrollBarValueChanged(int value)
{
	//qfLogFuncFrame() << "value:" << value;
	//qfInfo() << value;
	static int old_val = -1;
	QScrollBar *sb = verticalScrollBar();
	if(value == old_val) {
		if(value == sb->maximum()) {
			emit showNextPage();
		}
		else if(value == sb->minimum()) {
			emit showPreviousPage();
		}
	}
	old_val = value;
}

//====================================================
//                                 QFReportViewWidget::PainterWidget
//====================================================
QFReportViewWidget::PainterWidget::PainterWidget(QWidget *parent)
	: QWidget(parent)
{
	//screenDPMm = QPainter(this).device()->physicalDpiX() / 25.4;
	//setAutoFillBackground(true);
	//resize(210*2+20, 297*2+20);
}

QFReportViewWidget* QFReportViewWidget::PainterWidget::reportViewWidget()
{
	return qfFindParent<QFReportViewWidget*>(this);
}

void QFReportViewWidget::PainterWidget::paintEvent(QPaintEvent *ev)
{
	//qfTrash() << QF_FUNC_NAME;
	QWidget::paintEvent(ev);
	QFReportPainter painter(this);
	painter.setMarkEditableSqlText(true);

	/// nakresli ramecek a stranku
	//painter.setBrush(Qt::yellow);
	QRect r1 = rect();
	painter.fillRect(r1, QBrush(QColor("#CCFF99")));
	//int d = (int)(QFReportViewWidget::PageBorder * reportViewWidget()->scale());
	//r.adjust(d, d, -d, -d);
	/*
	QPen p(Qt::blue);
	p.setBrush(QColor("red"));
	//p.setWidth(2);
	painter.setPen(p);
	painter.drawRect(rect().adjusted(0, 0, -1, -1));
	painter.setBrush(Qt::green);
	QFont f("autobus", 30);
	f.setStyleHint(QFont::Times);
	painter.setFont(f);
	//painter.drawText(r, Qt::AlignCenter | Qt::TextWordWrap, "<qt>Qt <b>kjutyn</b> <br>indian</qt>");
	*/
	reportViewWidget()->setupPainter(&painter);
	QFReportItemMetaPaintFrame *frm = reportViewWidget()->currentPage();
	if(!frm) return;
	QFGraphics::Rect r = QFGraphics::mm2device(frm->renderedRect, painter.device());
	painter.fillRect(r, QColor("white"));
	painter.setPen(QColor("teal"));
	painter.setBrush(QBrush());
	painter.drawRect(r);

	painter.drawMetaPaint(reportViewWidget()->currentPage());
	//painter.setPen(p);
	//QRect r(0, 0, 210, 297);
	//painter.drawText(r, Qt::AlignCenter | Qt::TextWordWrap, "<qt>Qt <b>kjutyn</b> <br>indian</qt>");
}

void QFReportViewWidget::PainterWidget::mousePressEvent(QMouseEvent *e)
{
	qfLogFuncFrame();
	if(e->button() == Qt::LeftButton && e->modifiers() == Qt::ShiftModifier) {
		QWidget::mousePressEvent(e); /// ignoruj posouvani reportu mysi
	}
	else {
		QFReportItemMetaPaint::Point p = QPointF(e->pos());
		p = reportViewWidget()->painterInverseMatrix.map(p);
		p = QFGraphics::device2mm(p, this);
		qfTrash() << QF_FUNC_NAME << QFReportProcessorItem::Point(p).toString();
		reportViewWidget()->selectItem(p);
		QFReportItemMetaPaint *selected_item = reportViewWidget()->selectedItem();
		if(selected_item) {
			if(e->button() == Qt::RightButton) {
				//qfInfo() << "\t item type:" << typeid(*selected_item).name();
				QFReportItemMetaPaintText *it = dynamic_cast<QFReportItemMetaPaintText*>(selected_item->firstChild());
				if(it) {
					QMenu menu(this);
					menu.setTitle(tr("Item menu"));
					QAction *act_edit = menu.addAction(tr("Editovat text"));
					{
						QStringList edit_grants = QFString(it->editGrants).splitAndTrim(',');
						bool edit_enabled = edit_grants.isEmpty();
						QFApplication *app = qobject_cast<QFApplication*>(QCoreApplication::instance());
						qfTrash() << "app:" << app << "edit grants:" << edit_grants.join(",") << "edits enabled:" << edit_enabled;
						if(app) foreach(QString grant, edit_grants) if(app->currentUserHasGrant(grant)) {edit_enabled = true; break;}
						act_edit->setEnabled(edit_enabled);
					}
					QAction *a = menu.exec(mapToGlobal(e->pos()));
					if(a == act_edit) {
						ItemValueEditorWidget *w = new ItemValueEditorWidget();
						w->setValue(it->text);
						QFButtonDialog dlg(this);
						dlg.setDialogWidget(w);
						if(dlg.exec()) {
							QVariant val = w->value();
							it->text = val.toString();
							/// nevim proc, ale \n mi to tiskne, jako zvonecek :)
							it->text.replace('\n', QChar::LineSeparator);
							/// roztahni text na ohranicujici ramecek, aby se tam delsi text vesel
							it->setRenderedRectRect(selected_item->renderedRect);
							update();
							QString sql_id = it->sqlId;
							if(!sql_id.isEmpty()) {
								emit sqlValueEdited(sql_id, val);
							}
						}
					}
				}
			}
		}
	}
}

void QFReportViewWidget::PainterWidget::wheelEvent(QWheelEvent *event)
{
	/*
	if(event->modifiers() == Qt::ShiftModifier) {
		int delta = event->delta();
		//int numSteps = numDegrees / 15;

		if (event->orientation() == Qt::Horizontal) {
			//scrollHorizontally(numSteps);
		} else {
			emit zoomOnWheel(delta, event->pos());
		}
		event->accept();
	}
	else
		*/
		QWidget::wheelEvent(event);
}

//====================================================
//                                 QFReportViewWidget
//====================================================
QFReportViewWidget::QFReportViewWidget(QWidget *parent)
	: QFDialogWidget(parent), f_scrollArea(NULL), edCurrentPage(NULL), f_statusBar(NULL)
{
	f_reportProcessor = NULL;
	whenRenderingSetCurrentPageTo = -1;

	f_uiBuilder = new QFUiBuilder(this, ":/libqfgui/qfreportviewwidget.ui.xml");
	f_actionList += f_uiBuilder->actions();

	action("file.export.email")->setVisible(false);
	//action("report.edit")->setVisible(false);

	fCurrentPageNo = -1;
	f_selectedItem = NULL;

	f_scrollArea = new ScrollArea(NULL);
	/// zajimavy, odkomentuju tenhle radek a nemuzu nastavit pozadi zadnyho widgetu na scrollArea.
	//f_scrollArea->setBackgroundRole(QPalette::Dark);
	f_painterWidget = new PainterWidget(f_scrollArea);
	connect(f_painterWidget, SIGNAL(sqlValueEdited(QString,QVariant)), this, SIGNAL(sqlValueEdited(QString,QVariant)), Qt::QueuedConnection);
	connect(f_scrollArea, SIGNAL(zoomOnWheel(int,QPoint)), this, SLOT(zoomOnWheel(int,QPoint)), Qt::QueuedConnection);
	f_scrollArea->setWidget(f_painterWidget);
	QBoxLayout *ly = new QVBoxLayout(centralWidget());
	ly->setSpacing(0);
	ly->setMargin(0);
	ly->addWidget(f_scrollArea);
	ly->addWidget(statusBar());

	connect(f_scrollArea, SIGNAL(showNextPage()), this, SLOT(scrollToNextPage()));
	connect(f_scrollArea, SIGNAL(showPreviousPage()), this, SLOT(scrollToPrevPage()));

	QFUiBuilder::connectActions(f_actionList, this);
}

QFReportViewWidget::~QFReportViewWidget()
{
}

QFReportProcessor * QFReportViewWidget::reportProcessor()
{
	if(f_reportProcessor == NULL) {
		setReportProcessor(new QFReportProcessor(f_painterWidget, NULL, this));
	}
	return f_reportProcessor;
}

void QFReportViewWidget::setReportProcessor(QFReportProcessor * proc)
{
	f_reportProcessor = proc;
	connect(f_reportProcessor, SIGNAL(pageProcessed()), this, SLOT(pageProcessed()));
}

QFStatusBar* QFReportViewWidget::statusBar()
{
	if(!f_statusBar) {
		f_statusBar = new QFStatusBar(NULL);
		zoomStatusSpinBox = new QSpinBox();
		zoomStatusSpinBox->setSingleStep(10);
		zoomStatusSpinBox->setMinimum(10);
		zoomStatusSpinBox->setMaximum(1000000);
		zoomStatusSpinBox->setPrefix("zoom: ");
		zoomStatusSpinBox->setSuffix("%");
		zoomStatusSpinBox->setAlignment(Qt::AlignRight);
		f_statusBar->addWidget(zoomStatusSpinBox);
		connect(zoomStatusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setScaleProc(int)));
	}
	return f_statusBar;
}

QFPart::ToolBarList QFReportViewWidget::createToolBars()
{
	qfTrash() << QF_FUNC_NAME;
	QFPart::ToolBarList tool_bars;
	//if(!f_toolBars.isEmpty()) return f_toolBars;

	tool_bars = f_uiBuilder->createToolBars();

	/// je to dost divoky, najdu toolbar jmenem "main" a v nem widget majici akci s id == "view.nextPage"
	/// a pred ni prdnu lineEdit, pukud pridam do toolbaru widget, musim na konec pridat i Stretch :(
	foreach(QFToolBar *tb, tool_bars) {
		//qfTrash() << "\ttoolbar object name:" << tb->objectName();
		if(tb->objectName() == "main") {
			QLayout *ly = tb->layout();
			//QF_ASSERT(ly, "bad layout cast");
			for(int i = 0; i < ly->count(); ++i) {
				QWidget *w = ly->itemAt(i)->widget();
				//qfTrash() << "\twidget:" << w;
				if(w) {
					QList<QAction*> alst = w->actions();
					if(!alst.isEmpty()) {
						QFAction *a = qobject_cast<QFAction*>(alst[0]);
						if(a) {
							//qfTrash() << "\taction id:" << a->id();
							if(a->id() == "view.nextPage") {
								edCurrentPage = new QLineEdit(NULL);
								edCurrentPage->setAlignment(Qt::AlignRight);
								edCurrentPage->setMaximumWidth(60);
								connect(edCurrentPage, SIGNAL(editingFinished()), this, SLOT(edCurrentPageEdited()));
								tb->insertWidget(a, edCurrentPage);
								QLabel *space = new QLabel(QString(), NULL);
								space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
								tb->addWidget(space);
								//QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
								//ly->addItem(spacer);
/*
								ly->insertWidget(i, edCurrentPage);
								ly->addStretch();
								*/
								break;
							}
						}
					}
				}
			}
			break;
		}
	}

	return tool_bars;
}

void QFReportViewWidget::view_zoomIn(const QPoint &center_pos)
{
	qfLogFuncFrame() << "center_pos:" << center_pos.x() << center_pos.y();
	const QRect visible_rect(-f_scrollArea->widget()->pos(), f_scrollArea->viewport()->size());
	//QSizeF old_report_size = f_scrollArea->widget()->size();
	QPointF old_abs_center_pos = visible_rect.topLeft() + center_pos;
	qfTrash() << "visible rect:" << QFGraphics::Rect(visible_rect).toString();
	QScrollBar *hsb = f_scrollArea->horizontalScrollBar();
	QScrollBar *vsb = f_scrollArea->verticalScrollBar();

	qreal old_scale = scale(), new_scale;
	//QPointF old_report_pos = QPointF(abs_center_pos) / old_scale;
	//if(old_scale > 1) new_scale = old_scale + 1;
	//else if(old_scale > 1) new_scale = old_scale + 1;
	//else
	new_scale = old_scale * 1.33;
	setScale(new_scale);
	//qfInfo() << "new scale:" << new_scale << "old scale:" << old_scale;
	if(!center_pos.isNull()) {
		QPointF new_abs_center_pos = old_abs_center_pos * (new_scale / old_scale);
		if(hsb) {
			hsb->setValue(new_abs_center_pos.x() - center_pos.x());
			qfTrash() << "\t horizontal scrollbar value2:" << hsb->value() << "of" << hsb->maximum();
		}
		if(vsb) {
			vsb->setValue(new_abs_center_pos.y() - center_pos.y());
			qfTrash() << "\t vertical scrollbar value2:" << vsb->value() << "of" << vsb->maximum();
		}
	}
}

void QFReportViewWidget::view_zoomOut(const QPoint &center_pos)
{
	qfLogFuncFrame() << "center_pos:" << center_pos.x() << center_pos.y();
	const QRect visible_rect(-f_scrollArea->widget()->pos(), f_scrollArea->viewport()->size());
	//QSizeF old_report_size = f_scrollArea->widget()->size();
	QPointF old_abs_center_pos = visible_rect.topLeft() + center_pos;
	qfTrash() << "visible rect:" << QFGraphics::Rect(visible_rect).toString();
	QScrollBar *hsb = f_scrollArea->horizontalScrollBar();
	QScrollBar *vsb = f_scrollArea->verticalScrollBar();

	qreal old_scale = scale(), new_scale;
	//QPointF old_report_pos = QPointF(abs_center_pos) / old_scale;
	//if(old_scale > 2) new_scale = old_scale - 1;
	//else
	new_scale = old_scale / 1.33;
	setScale(new_scale);

	if(!center_pos.isNull()) {
		QPointF new_abs_center_pos = old_abs_center_pos * (new_scale / old_scale);
		if(hsb) {
			hsb->setValue(new_abs_center_pos.x() - center_pos.x());
			qfTrash() << "\t horizontal scrollbar value2:" << hsb->value() << "of" << hsb->maximum();
		}
		if(vsb) {
			vsb->setValue(new_abs_center_pos.y() - center_pos.y());
			qfTrash() << "\t vertical scrollbar value2:" << vsb->value() << "of" << vsb->maximum();
		}
	}
}

void QFReportViewWidget::zoomOnWheel(int delta, const QPoint &center_pos)
{
	if(delta < 0) view_zoomIn(center_pos);
	else if(delta > 0) view_zoomOut(center_pos);
}

void QFReportViewWidget::view_zoomToFitWidth()
{
	qfTrash() << QF_FUNC_NAME;
	QFReportItemMetaPaintFrame *frm = currentPage();
	if(!frm) return;
	QFReportItemMetaPaintFrame::Rect r = frm->renderedRect;
	double report_px = (r.width() + 2*PageBorder) * logicalDpiX() / 25.4;
	double widget_px = f_scrollArea->width();
	//QScrollBar *sb = f_scrollArea->verticalScrollBar();
	//if(sb) widget_px -= sb->width();
	double sc = widget_px / report_px * 0.98;
	setScale(sc);
}

void QFReportViewWidget::view_zoomToFitHeight()
{
	QFReportItemMetaPaintFrame *frm = currentPage();
	if(!frm) return;
	QFReportItemMetaPaintFrame::Rect r = frm->renderedRect;
	double report_px = (r.height() + 2*PageBorder) * f_painterWidget->logicalDpiY() / 25.4;
	double widget_px = f_scrollArea->height();
	double sc = widget_px / report_px * 0.98;
	setScale(sc);
}

void QFReportViewWidget::setScale(qreal _scale)
{
	qfTrash() << QF_FUNC_NAME << currentPageNo();
	QFReportItemMetaPaintFrame *frm = currentPage();
	if(!frm) return;

	f_scale = _scale;
	setupPainterWidgetSize();
	f_painterWidget->update();
	refreshWidget();
}

void QFReportViewWidget::setupPainterWidgetSize()
{
	qfTrash() << QF_FUNC_NAME;
	QFReportItemMetaPaintFrame *frm = currentPage();
	if(!frm) return;
	QFGraphics::Rect r1 = frm->renderedRect.adjusted(-PageBorder, -PageBorder, PageBorder, PageBorder);
	QFGraphics::Rect r2 = QFGraphics::mm2device(r1, f_painterWidget);
	//qfTrash() << "\tframe rect:" << r.toString();
	QSizeF s = r2.size();
	s *= scale();
	//painterScale = QSizeF(s.width() / r1.width(), s.height() / r1.height());
	f_painterWidget->resize(s.toSize());
}

void QFReportViewWidget::setupPainter(QFReportPainter *p)
{
	QF_ASSERT(p, "painter is NULL");
	//qfInfo() << QF_FUNC_NAME;
	//qfInfo() << "\t painterScale:" << QFReportProcessorItem::Size(painterScale).toString();
	//p->currentPage = currentPageNo();
	p->pageCount = pageCount();
	//QFDomElement el;
	//if(f_selectedItem) el = f_selectedItem->reportElement;
	p->setSelectedItem(f_selectedItem);
	//if(f_selectedItem) qfInfo() << "painter selected item:" << f_selectedItem->reportItem()->path().toString() << f_selectedItem->reportItem()->element.tagName();
	p->scale(scale(), scale());
	//p->scale(painterScale.width(), painterScale.height());
	//qfInfo() << "\t painter world matrix m11:" << p->worldMatrix().m11() << "m12:" << p->worldMatrix().m12();
	//qfInfo() << "\t painter world matrix m21:" << p->worldMatrix().m21() << "m22:" << p->worldMatrix().m22();
	p->translate(QFGraphics::mm2device(QFGraphics::Point(PageBorder, PageBorder), p->device()));
	painterInverseMatrix = p->matrix().inverted();
}
/*
void QFReportViewWidget::setDocument(QFReportItemMetaPaint* doc)
{
	qfTrash() << QF_FUNC_NAME;
	fDocument = doc;
	if(!doc) return;
	//doc->dump();
}
	*/
void QFReportViewWidget::setReport(const QString &file_name)
{
	qfLogFuncFrame() << "file_name:" << file_name;
	//qfTrash() << "\tdata:" << fData.toString();
	reportProcessor()->setReport(file_name);
	reportProcessor()->setData(fData);
	//out.dump();
}

void QFReportViewWidget::setReport(const QFDomDocument & doc)
{
	reportProcessor()->setReport(doc);
	reportProcessor()->setData(fData);
}

void QFReportViewWidget::setData(const QFDomDocument &_data)
{
	qfLogFuncFrame();
	QDomElement el = _data.firstChildElement();
	QFXmlTable xt(el);
	qfTrash() << xt.toString();
	QFTreeTable tt = xt.toTreeTable();
	//qfInfo() << tt.toString(2);
	setData(tt);
}

void QFReportViewWidget::pageProcessed()
{
	qfTrash() << QF_FUNC_NAME;
	if(whenRenderingSetCurrentPageTo >= 0) {
		if(pageCount() - 1 == whenRenderingSetCurrentPageTo) {
			setCurrentPageNo(whenRenderingSetCurrentPageTo);
			whenRenderingSetCurrentPageTo = -1;
		}
	}
	else {
		if(pageCount() == 1) setCurrentPageNo(0);
	}
	//QApplication::processEvents();
	refreshWidget();
	//setCurrentPageNo(0);
	QTimer::singleShot(10, reportProcessor(), SLOT(processSinglePage())); /// 10 je kompromis mezi rychlosti prekladu a sviznosti GUI
}

QFReportItemMetaPaintReport* QFReportViewWidget::document(bool throw_exc) throw(QFException)
{
	QFReportItemMetaPaintReport *doc = reportProcessor()->processorOutput();
	if(!doc && throw_exc) QF_EXCEPTION("document is NULL");
	return doc;
}

int QFReportViewWidget::pageCount()
{
	//qfLogFuncFrame();
	int ret = 0;
	if(document(!Qf::ThrowExc)) {
		ret = document()->childrenCount();
	}
	else {
		//qfTrash() << "\tdocument is null";
	}
	//qfTrash() << "\treturn:" << ret;
	return ret;
}

void QFReportViewWidget::setCurrentPageNo(int pg_no)
{
	if(pg_no >= pageCount() || pg_no < 0) pg_no = 0;
	fCurrentPageNo = pg_no;
	setupPainterWidgetSize();
	f_painterWidget->update();
	refreshWidget();
}

QFReportItemMetaPaintFrame* QFReportViewWidget::getPage(int n)
{
	//qfTrash() << QF_FUNC_NAME << currentPageNo();
	if(!document(!Qf::ThrowExc)) return NULL;
	if(n < 0 || n >= document()->childrenCount()) return NULL;
	QFReportItemMetaPaint *it = document()->child(n);
	QFReportItemMetaPaintFrame *frm = dynamic_cast<QFReportItemMetaPaintFrame*>(it);
	//qfTrash() << "\treturn:" << frm;
	return frm;
}

QFReportItemMetaPaintFrame* QFReportViewWidget::currentPage()
{
	QFReportItemMetaPaintFrame *frm = getPage(currentPageNo());
	//qfTrash() << QF_FUNC_NAME << currentPageNo();
	if(!frm) return NULL;
	return frm;
}

void QFReportViewWidget::selectElement_helper(QFReportItemMetaPaint *it, const QFDomElement &el)
{
	QDomElement el1 = it->reportItem()->element;
	if(el1 == el) {
		//qfInfo() << "BINGO";
		f_selectedItem = it;
		qfTrash() << "\t EMIT:" << el1.tagName();
		emit elementSelected(el1);
	}
	else {
		foreach(QFTreeItemBase *_it, it->children()) {
			QFReportItemMetaPaint *it1 = static_cast<QFReportItemMetaPaint*>(_it);
			selectElement_helper(it1, el);
		}
	}
}

void QFReportViewWidget::selectElement(const QFDomElement &el)
{
	qfLogFuncFrame() << el.tagName();
	if(!el) return;
	//qfInfo() << __LINE__;
	#if 0
	kdyz se tohle odkomentuje, tak to pada
	if(f_selectedItem && f_selectedItem->reportItem()->element == el) {
		qfTrash() << "\t allready selected";
		return;
	}
	#endif
	f_selectedItem = NULL;
	QFReportItemMetaPaintFrame *frm = currentPage();
	if(!frm) return;
	selectElement_helper(frm, el);
	f_painterWidget->update();
}

static QString is_fake_element(const QFDomElement &_el)
{
	qfLogFuncFrame();
	QString fake_path;
	QFDomElement el = _el;
	while(!!el) {
		//qfError() << el.toString();
		fake_path = el.attribute("__fakePath");
		qfTrash() << "\t fake path of" << el.tagName() << "=" << fake_path;
		if(!fake_path.isEmpty()) break;
		else if(el.hasAttribute("__fake")) {
			fake_path = "-";
			break;
		}
		el = el.parentNode().toElement();
	}
	qfTrash() << "\t return:" << fake_path;
	return fake_path;
}

bool QFReportViewWidget::selectItem_helper(QFReportItemMetaPaint *it, const QPointF &p)
{
	bool ret = false;
	if(it->isPointInside(p)) {
		ret = true;
		qfLogFuncFrame() << "point inside:" << it->reportItem()->element.tagName() << it->renderedRect.toString();
		//qfInfo() << it->dump();
		bool in_child = false;
		foreach(QFTreeItemBase *_it, it->children()) {
			QFReportItemMetaPaint *it1 = static_cast<QFReportItemMetaPaint*>(_it);
			if(selectItem_helper(it1, p)) {in_child = true; break;}
		}
		if(!in_child) {
			//bool selectable_element_found = false;
			if(f_selectedItem != it) {
				f_selectedItem = it;
				//qfInfo() << "selected item:" << f_selectedItem->reportItem()->path().toString() << f_selectedItem->reportItem()->element.tagName();
				/// muze se stat, ze item nema element, pak hledej u rodicu
				while(it) {
					QFReportProcessorItem *r_it = it->reportItem();
					if(r_it) {
						f_painterWidget->update();
						/// pokus se najit nejaky rozumny element pro tento report item
						QFDomElement el = r_it->element;
						if(!!el) {
							if(el.attribute("unselectable").toBool()) { ret = false; break; }
							/// nalezeny element nesmi pochazet z fakeBand, pozna se to tak, ze element nebo nektery z jeho predku ma atribut "__fake"
							QString fake_path = is_fake_element(el);
							//qfInfo() << "fake path:" << fake_path;
							if(!fake_path.isEmpty()) {
								/// pokud je to fake element, bylo by fajn, kdyby se emitoval element, ze ktereho byl fake element vytvoren
								/// najdi element table
								QFReportItemTable *t_it = NULL;
								while(r_it) {
									t_it = dynamic_cast<QFReportItemTable*>(r_it);
									if(t_it) break;
									r_it = r_it->parent();
								}
								QDomElement faked_el;
								if(t_it) {
									if(fake_path == "-") {
										faked_el = t_it->element;
									}
									else {
										//qfInfo() << "cd" << fake_path;
										el = t_it->element;
										faked_el = el.cd(fake_path, !Qf::ThrowExc);
										/*
										if(!faked_el.isNull()) {
											qfInfo() << "\tOK";
											//selectable_element_found = true;
											//qfTrash() << "element:" << fSelectedElement.toString();
										}
										*/
									}
									qfTrash() << "\t EMIT:" << faked_el.tagName();
									emit elementSelected(faked_el);
								}
							}
							else {
								//selectable_element_found = true;
								//qfTrash() << "element:" << fSelectedElement.toString();
								qfTrash() << "\t EMIT:" << el.tagName();
								emit elementSelected(el);
							}
						}
					}
					else {
						//qfWarning() << "item for path:" << it->path().toString() << "NOT FOUND.";
					}
					break;
					//it = it->parent();
				}
			}
			//return true;
		}
		//return true;
	}
	return ret;
}

void QFReportViewWidget::selectItem(const QPointF &p)
{
	qfLogFuncFrame();
	QFReportItemMetaPaintFrame *frm = currentPage();
	QFReportItemMetaPaint *old_selected_item = f_selectedItem;
	//QFDomElement old_el = fSelectedElement;
	f_selectedItem = NULL;
	if(frm) selectItem_helper(frm, p);
	if(!f_selectedItem && old_selected_item) {
		/// odznac puvodni selekci
		f_painterWidget->update();
	}
}

void QFReportViewWidget::setVisible(bool visible)
{
	qfTrash() << QF_FUNC_NAME;
	//setCurrentPageNo(0);
	QFDialogWidget::setVisible(visible);
	//setCurrentPageNo(0);
	//QTimer::singleShot(0, this, SLOT(processReport()));
	if(visible) processReport();
}

void QFReportViewWidget::processReport()
{
	qfLogFuncFrame();
	if(!reportProcessor()->processorOutput()) {
		reportProcessor()->process(QFReportProcessor::FirstPage);
	}
	setCurrentPageNo(0);
	setScale(1);
	//fDocument = reportProcessor()->processorOutput();
}

void QFReportViewWidget::render()
{
	qfLogFuncFrame();
	whenRenderingSetCurrentPageTo = currentPageNo();
	reportProcessor()->reset();
	if(!reportProcessor()->processorOutput()) {
		//qfInfo() << "process report";
		reportProcessor()->process(QFReportProcessor::FirstPage);
	}
	//qfInfo() << "setCurrentPageNo:" << cur_page_no;
	//setCurrentPageNo(cur_page_no);
}

void QFReportViewWidget::refreshWidget()
{
	statusBar();
	if(edCurrentPage) edCurrentPage->setText(QString::number(currentPageNo()+1) + "/" + QString::number(pageCount()));
	refreshActions();
	zoomStatusSpinBox->setValue((int)(scale() * 100));
	//statusBar()->setText("zoom: " + QString::number((int)(scale() * 100)) + "%");
}

void QFReportViewWidget::refreshActions()
{
	int pgno = currentPageNo();
	int pgcnt = pageCount();
	action("view.firstPage")->setEnabled(pgno > 0 && pgcnt > 0);
	action("view.prevPage")->setEnabled(pgno > 0 && pgcnt > 0);
	action("view.nextPage")->setEnabled(pgno < pgcnt - 1);
	action("view.lastPage")->setEnabled(pgno < pgcnt - 1);
}

void QFReportViewWidget::view_nextPage(PageScrollPosition scroll_pos)
{
	qfTrash() << QF_FUNC_NAME;
	if(currentPageNo() < pageCount() - 1) {
		setCurrentPageNo(currentPageNo() + 1);
		if(scroll_pos == ScrollToPageTop) f_scrollArea->verticalScrollBar()->setValue(f_scrollArea->verticalScrollBar()->minimum());
		else if(scroll_pos == ScrollToPageEnd) f_scrollArea->verticalScrollBar()->setValue(f_scrollArea->verticalScrollBar()->maximum());
	}
}

void QFReportViewWidget::view_prevPage(PageScrollPosition scroll_pos)
{
	qfTrash() << QF_FUNC_NAME;
	if(currentPageNo() > 0) {
		setCurrentPageNo(currentPageNo() - 1);
		if(scroll_pos == ScrollToPageTop) f_scrollArea->verticalScrollBar()->setValue(f_scrollArea->verticalScrollBar()->minimum());
		else if(scroll_pos == ScrollToPageEnd) f_scrollArea->verticalScrollBar()->setValue(f_scrollArea->verticalScrollBar()->maximum());
	}
}

void QFReportViewWidget::view_firstPage()
{
	qfTrash() << QF_FUNC_NAME;
	int pgno = 0;
	if(pgno < pageCount()) setCurrentPageNo(pgno);
}

void QFReportViewWidget::view_lastPage()
{
	qfTrash() << QF_FUNC_NAME;
	int pgno = pageCount() - 1;
	if(pgno >= 0) setCurrentPageNo(pgno);
}

void QFReportViewWidget::edCurrentPageEdited()
{
	qfTrash() << QF_FUNC_NAME;
	QStringList sl = edCurrentPage->text().split("/");
	if(sl.count() > 0) {
		int pg = sl[0].toInt() - 1;
		setCurrentPageNo(pg);
	}
}

void QFReportViewWidget::print(QPrinter *printer) throw(QFException)
{
	if(printer) print(*printer);
}

void QFReportViewWidget::print(QPrinter &printer, const QVariantMap &options) throw(QFException)
{
	qfLogFuncFrame();

	QFCursorOverrider cov(Qt::WaitCursor);
	//QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	QFReportPainter painter(&printer);

	typedef QFReportProcessorItem::Rect Rect;
	//typedef QFReportProcessorItem::Size Size;

	int pg_no = options.value("fromPage", 1).toInt() - 1;
	int to_page = options.value("toPage", pageCount()).toInt();
	qfTrash() << "pg_no:" << pg_no << "to_page:" << to_page;
	QFReportItemMetaPaintFrame *frm = getPage(pg_no);
	if(frm) {
		Rect r = frm->renderedRect;
		bool landscape = r.width() > r.height();
		if(landscape) printer.setOrientation(QPrinter::Landscape);
		//Rect printer_pg_rect = QRectF(printer.pageRect());
		//qfWarning() << "\tprinter page rect:" << printer_pg_rect.toString();
		//qfWarning() << "\tresolution:" << printer.resolution() << Size(printer_pg_rect.size()/printer.resolution()).toString(); /// resolution je v DPI
		//qreal magnify = printer_pg_rect.width() / r.width();
		//painter.scale(magnify, magnify);
		painter.pageCount = pageCount();
		while(frm) {
			//painter.currentPage = pg_no;
			painter.drawMetaPaint(frm);
			pg_no++;
			frm = getPage(pg_no);
			if(!frm) break;
			if(pg_no >= to_page) break;
			printer.newPage();
		}
	}

	//QApplication::restoreOverrideCursor();
	//emit reportPrinted();
}

void QFReportViewWidget::print() throw(QFException)
{
	qfLogFuncFrame();

	QPrinter printer;
	printer.setOutputFormat(QPrinter::NativeFormat);
	//printer.setOutputFileName(fn);
	printer.setFullPage(true);
	printer.setPageSize(QPrinter::A4);
	printer.setOrientation(document()->orientation);

	QPrintDialog dlg(&printer, this);
	if(dlg.exec() != QDialog::Accepted) return;

	qfTrash() << "options:" << dlg.options();
	QVariantMap opts;
	if(dlg.testOption(QAbstractPrintDialog::PrintPageRange)) { /// tohle je nastaveny vzdycky :(
		int from_page = dlg.fromPage();
		int to_page = dlg.toPage();
		qfTrash() << "fromPage:" << dlg.fromPage() << "toPage:" << dlg.toPage();
		if(from_page > 0) opts["fromPage"] = dlg.fromPage();
		if(to_page > 0) opts["toPage"] = dlg.toPage();
	}

	print(printer, opts);
}

void QFReportViewWidget::exportPdf(const QString &file_name) throw(QFException)
{
	qfTrash() << QF_FUNC_NAME;
	QFString fn = file_name;
	if(!fn) QF_EXCEPTION(tr("empty file name"));
	if(!fn.toLower().endsWith(".pdf")) fn += ".pdf";

	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fn);
	printer.setFullPage(true);
	printer.setPageSize(QPrinter::A4);
	printer.setOrientation(document()->orientation);
	/// zatim bez dialogu
	//QPrintDialog printDialog(&printer, parentWidget());
	//if(printDialog.exec() != QDialog::Accepted) return;

	print(printer);
}

QString QFReportViewWidget::exportHtml() throw( QFException )
{
	qfLogFuncFrame();
	QFDomDocument doc = QFHtmlUtils::bodyToHtmlDocument(QString());
	QFDomElement el_body = doc.cd("/body");
	QFCursorOverrider cov(Qt::WaitCursor);
	//QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	reportProcessor()->processHtml(el_body);
	//QApplication::restoreOverrideCursor();
	return doc.toString(2, false);
}

void QFReportViewWidget::file_export_pdf(bool open)
{
	qfTrash() << QF_FUNC_NAME;
	attachPrintout();
	//reportProcessor()->dump();
	QFString fn;
	if(open) fn = qfApp()->tempDir() + "/report.pdf";
	else fn = qfApp()->getSaveFileName (this, tr("Save as PDF"), QString(), "*.pdf");
	if(!fn) return;
	if(!fn.endsWith(".pdf", Qt::CaseInsensitive)) fn += ".pdf";
	exportPdf(fn);
	if(open) {
		QDesktopServices::openUrl(QUrl::fromLocalFile(fn));
	}
}

void QFReportViewWidget::file_export_html()
{
	qfLogFuncFrame();
	QFString fn = "report.html";
	QString s = exportHtml();
	QFHtmlViewUtils hut(!QFHtmlViewUtils::UseWebKit);
	hut.showOrSaveHtml(s, fn, "showOrSaveHtml");
}

void QFReportViewWidget::file_export_email()
{
	/// uloz pdf do tmp file report.pdf
	attachPrintout();
	QString fn = qfApp()->tempDir() + "/report.pdf";
	exportPdf(fn);
}

void QFReportViewWidget::data_showHtml()
{
	qfTrash() << QF_FUNC_NAME;
	QString s = reportProcessor()->data().toHtml();
	s = QFHtmlUtils::addHtmlEnvelope(s);
	QString file_name = "data.html";

	QFHtmlViewUtils hu(!QFHtmlViewUtils::UseWebKit);
	hu.showOrSaveHtml(s, file_name);
}

void QFReportViewWidget::file_print()
{
	qfTrash() << QF_FUNC_NAME;
	attachPrintout();
	print();
}

void QFReportViewWidget::file_printPreview()
{
	qfLogFuncFrame();
	QPrinter printer(QPrinter::ScreenResolution);	/// SVG to tiskne na preview moc velky, pokud je QPrinter printer(QPrinter::HighResolution), je to potreba opravit
	QPrintPreviewDialog preview(&printer, this);
	connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
	preview.exec();
}

QFDataTranslator* QFReportViewWidget::dataTranslator() const
{
	QFDataTranslator *ret = NULL;
	{
		QFAppDataTranslatorInterface *appi = dynamic_cast<QFAppDataTranslatorInterface *>(QCoreApplication::instance());
		if(appi) {
			ret = appi->dataTranslator();
		}
	}
	return ret;
}

void QFReportViewWidget::report_edit()
{
	qfLogFuncFrame();

	QString program = QFFileUtils::appDir() + "/repedit";
	#if defined Q_OS_WIN
	program += ".exe";
	#endif
	QStringList arguments;
	{
		QFSqlSearchDirs *sql_sd = dynamic_cast<QFSqlSearchDirs*>(reportProcessor()->searchDirs(!Qf::ThrowExc));
		if(sql_sd) {
			QFAppDbConnectionInterface *appi = dynamic_cast<QFAppDbConnectionInterface*>(QCoreApplication::instance());
			if(appi) {
				QFSqlConnection &c = appi->connection();
				/// user:password@host:port
				QString sql_connection = c.userName() % ':' % c.password() % '@' % c.hostName() % ':' % QString::number(c.port());
				/*
				QString sql_connection = "sql://" % c.userName() % ':' % c.password() % '@' % c.hostName() % ':' % QString::number(c.port())
				% '/' % c.databaseName()
				% '/' % sql_sd->tableName()
				% '/' % sql_sd->ckeyColumnName()
				% '/' % sql_sd->dataColumnName();
				*/
				arguments << "--sql-connection=" + sql_connection;
				//arguments << "--dbfs-profile=" + sql_connection;
				arguments << "--report-search-sql-dirs=" + sql_sd->sqlDirs().join("::");
			}
		}
	}
	{
		QFSearchDirs *sd = dynamic_cast<QFSearchDirs*>(reportProcessor()->searchDirs(!Qf::ThrowExc));
		if(sd) {
			arguments << "--report-search-dirs=" + sd->dirs().join("::");
			if(sd->dirs().count()) arguments << "--saved-files-root-dir=" + sd->dirs().first();
		}
	}
	QFDataTranslator *dtr = dataTranslator();
	if(dtr) {
		QString lc_domain = dtr->currentLocalesName();
		if(!lc_domain.isEmpty()) arguments << "--force-lc-domain=" + lc_domain;
	}
	arguments << reportProcessor()->report().fileName();
	//arguments << "-dqfdom";
#if defined QT_DEBUG /// aby se mi netisklo heslo do logu v release verzi
	qfDebug() << "\t command line:" << program << arguments.join(" ");
#endif
	QProcess *proc = new QProcess(qfApp());
	proc->start(program, arguments);
}





