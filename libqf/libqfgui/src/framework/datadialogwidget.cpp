#include "datadialogwidget.h"
#include "../dialogs/dialog.h"

#include <qf/gui/model/datadocument.h>
#include <qf/core/log.h>

namespace qfm = qf::gui::model;
using namespace qf::gui::framework;

DataDialogWidget::DataDialogWidget(QWidget *parent)
	: Super(parent)
{
}

DataDialogWidget::~DataDialogWidget() = default;

qf::gui::DataController *DataDialogWidget::dataController()
{
	if(!m_dataController) {
		m_dataController = new qf::gui::DataController(this);
		m_dataController->setWidget(this);
	}
	return m_dataController;
}

void DataDialogWidget::setDataController(qf::gui::DataController *dc)
{
	m_dataController = dc;
}

qf::gui::model::DataDocument *DataDialogWidget::dataDocument(bool throw_exc)
{
	qf::gui::DataController *dc = dataController();
	return dc->document(throw_exc);
}

bool DataDialogWidget::load(const QVariant &id, int mode)
{
	auto *doc = dataDocument(!qf::core::Exception::Throw);
	if(doc) {
		connect(doc, &qfm::DataDocument::saved, this, &DataDialogWidget::dataSaved, Qt::UniqueConnection);
		bool ok = doc->load(id, qf::gui::model::DataDocument::RecordEditMode(mode));
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
	if(result == qf::gui::dialogs::Dialog::ResultAccept) {
		auto mode = recordEditMode();
		if(mode == qf::gui::model::DataDocument::ModeDelete) {
			ret = dropData();
		}
		else if(mode == qf::gui::model::DataDocument::ModeEdit
				|| mode == qf::gui::model::DataDocument::ModeInsert
				|| mode == qf::gui::model::DataDocument::ModeCopy) {
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

