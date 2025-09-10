#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qf/gui/framework/mainwindow.h>

class MainWindow : public qf::gui::framework::MainWindow
{
	Q_OBJECT
private:
	using Super = qf::gui::framework::MainWindow;
public:
	explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~MainWindow() override;

	void loadPlugins() override;
protected:
	void onPluginsLoaded() Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
