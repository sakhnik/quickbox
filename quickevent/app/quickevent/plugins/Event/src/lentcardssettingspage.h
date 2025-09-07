#pragma once

#include <qf/gui/framework/dialogwidget.h>

#include <plugins/Core/src/widgets/settingspage.h>

namespace Ui {
class LentCardsSettingsPage;
}

namespace qf::gui::model { class SqlTableModel; }

class LentCardsSettingsPage : public Core::SettingsPage
{
	Q_OBJECT
private:
	using Super = Core::SettingsPage;

public:
	explicit LentCardsSettingsPage(QWidget *parent = 0);
	~LentCardsSettingsPage() override;
protected:
	void load() override;
	void save() override;

private:
	Ui::LentCardsSettingsPage *ui;
	qf::gui::model::SqlTableModel *m_tableModel;
};

