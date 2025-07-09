#pragma once

#include <qf/core/utils.h>

#include <QVariantMap>
#include <QTime>
#include <QDate>

namespace Event {


class StageData : public QVariantMap
{
private:
	typedef QVariantMap Super;

	QF_VARIANTMAP_KEY_FIELD(bool, useallmaps, is, set, UseAllMaps)
	QF_VARIANTMAP_KEY_FIELD(QVariantMap, drawingconfig, d, setD, rawingConfig)
	QF_VARIANTMAP_KEY_FIELD(QString, qxapitoken, qx, setQx, ApiToken)
public:
	StageData(const QVariantMap &data = QVariantMap());

	QDateTime startDateTime() const;
};

}

