#ifndef QF_GUI_PROGRESSBAR_H
#define QF_GUI_PROGRESSBAR_H

#include "guiglobal.h"

#include <QProgressBar>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT ProgressBar : public QProgressBar
{
	Q_OBJECT
private:
	typedef QProgressBar Super;
public:
	explicit ProgressBar(QWidget *parent = nullptr);
};

}}

#endif // PROGRESSBAR_H
