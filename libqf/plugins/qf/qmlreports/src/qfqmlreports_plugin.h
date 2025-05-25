#pragma once

#include <QQmlExtensionPlugin>

QT_BEGIN_NAMESPACE

class QFQmlReportPlugin : public QQmlExtensionPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
	QFQmlReportPlugin(QObject *parent = nullptr) : QQmlExtensionPlugin(parent) { }
	void registerTypes(const char *uri) override;
};

QT_END_NAMESPACE

