#include "datadialogwidget.h"
#include "../dialogs/dialog.h"

#include <qf/qmlwidgets/model/datadocument.h>
#include <qf/core/log.h>

namespace qfm = qf::qmlwidgets::model;
using namespace qf::qmlwidgets::framework;

DataDialogWidget::DataDialogWidget(QWidget *parent)
	: Super(parent)
{
}

DataDialogWidget::~DataDialogWidget() = default;

qf::qmlwidgets::DataController *DataDialogWidget::dataController()
{
	if(!m_dataController) {
		m_dataController = new qf::qmlwidgets::DataController(this);
		m_dataController->setWidget(this);
	}
	return m_dataController;
}

void DataDialogWidget::setDataController(qf::qmlwidgets::DataController *dc)
{
	m_dataController = dc;
}

qf::qmlwidgets::model::DataDocument *DataDialogWidget::dataDocument(bool throw_exc)
{
	qf::qmlwidgets::DataController *dc = dataController();
	return dc->document(throw_exc);
}

bool DataDialogWidget::load(const QVariant &id, int mode)
{
	auto *doc = dataDocument(!qf::core::Exception::Throw);
	if(doc) {
		connect(doc, &qfm::DataDocument::saved, this, &DataDialogWidget::dataSaved, Qt::UniqueConnection);
		bool ok = doc->load(id, qf::qmlwidgets::model::DataDocument::RecordEditMode(mode));
		if(ok)
			emit recordEditModeChanged(mode);
		return ok;
	}
	return false;
}

bool DataDialogWidget::acceptDialogDone(int result)
{
	qfLogFuncFrame();
	bool ret = true;
	if(result == qf::qmlwidgets::dialogs::Dialog::ResultAccept) {
		auto mode = recordEditMode();
		if(mode == qf::qmlwidgets::model::DataDocument::ModeDelete) {
			ret = dropData();
		}
		else if(mode == qf::qmlwidgets::model::DataDocument::ModeEdit
				|| mode == qf::qmlwidgets::model::DataDocument::ModeInsert
				|| mode == qf::qmlwidgets::model::DataDocument::ModeCopy) {
			ret = saveData();
		}
	}
	if(ret)
		ret = Super::acceptDialogDone(result);
	return ret;
}

int DataDialogWidget::recordEditMode()
{
	int ret = -1;
	if(m_dataController && m_dataController->document(false)) {
		ret = m_dataController->document()->mode();
	}
	return ret;
}

bool DataDialogWidget::saveData()
{
	bool ret = false;
	DataController *dc = dataController();
	if(dc) {
		qfm::DataDocument *doc = dc->document(false);
		if(doc) {
			ret = doc->save();
		}
	}
	return ret;
}

bool DataDialogWidget::dropData()
{
	bool ret = false;
	DataController *dc = dataController();
	if(dc) {
		qfm::DataDocument *doc = dc->document(false);
		if(doc) {
			ret = doc->drop();
		}
	}
	return ret;
}

