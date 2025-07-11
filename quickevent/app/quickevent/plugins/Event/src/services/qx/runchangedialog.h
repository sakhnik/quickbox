#ifndef RUNCHANGEDIALOG_H
#define RUNCHANGEDIALOG_H

#include <QDialog>

namespace Event::services::qx {

namespace Ui {
class RunChangeDialog;
}

struct RunChange;
class QxClientService;

class RunChangeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit RunChangeDialog(int change_id, int run_id, int lock_number, const RunChange &run_change, QWidget *parent = nullptr);
	~RunChangeDialog() override;

private:
	QxClientService* service();
	void setMessage(const QString &msg = {}, bool error = false);

	void loadOrigValues();
	void lockChange();

private:
	Ui::RunChangeDialog *ui;
	int m_changeId = 0;
	int m_runId = 0;
};


} // namespace Event::services::qx


#endif // RUNCHANGEDIALOG_H
