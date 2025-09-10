#ifndef QF_CORE_QML_SETTINGS_H
#define QF_CORE_QML_SETTINGS_H

#include <qf/core/utils/settings.h>

namespace qf {
namespace core {
namespace qml {

class Settings : public qf::core::utils::Settings
{
	Q_OBJECT
private:
	typedef qf::core::utils::Settings Super;
public:
	explicit Settings(QObject *parent = nullptr);
	~Settings() Q_DECL_OVERRIDE;
public:
	Q_INVOKABLE void beginGroup(const QString & prefix) {Super::beginGroup(prefix);}
	Q_INVOKABLE int beginReadArray(const QString & prefix) {return Super::beginReadArray(prefix);}
	Q_INVOKABLE void beginWriteArray(const QString & prefix, int size = -1) {Super::beginWriteArray(prefix, size);}
	Q_INVOKABLE void endArray() {Super::endArray();}
	Q_INVOKABLE void endGroup() {Super::endGroup();}
	Q_INVOKABLE void setArrayIndex(int i) {Super::setArrayIndex(i);}
	Q_INVOKABLE QString group() const {return Super::group();}
	Q_INVOKABLE void sync() {Super::sync();}
};

}}}

#endif
