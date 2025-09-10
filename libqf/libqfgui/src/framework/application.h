#ifndef QF_GUI_FRAMEWORK_APPLICATION_H
#define QF_GUI_FRAMEWORK_APPLICATION_H

#include "../guiglobal.h"

#include <QApplication>
#include <QJsonDocument>
#include <QQmlError>

class QQmlEngine;

namespace qf {
namespace gui {
namespace framework {

class MainWindow;

class QFGUI_DECL_EXPORT Application : public QApplication
{
	Q_OBJECT
	friend class MainWindow;
private:
	typedef QApplication Super;
public:
	explicit Application(int & argc, char ** argv);
	~Application() override;

	Q_INVOKABLE QString versionString() const;
public:
	static Application* instance(bool must_exist = true);
	MainWindow* frameWork();

	void loadStyleSheet(const QString &file = QString());
public slots:
	QString applicationDirPath();
	QString applicationName();
	QStringList arguments();
protected:
	QJsonDocument profile();
protected:
	QJsonDocument m_profile;
	bool m_profileLoaded = false;
	MainWindow* m_frameWork = nullptr;
};

}}}

#endif // QF_GUI_FRAMEWORK_APPLICATION_H
