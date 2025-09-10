#ifndef QF_GUI_DIALOGBUTTONBOX_H
#define QF_GUI_DIALOGBUTTONBOX_H

#include "guiglobal.h"

#include <QDialogButtonBox>

namespace qf {
namespace gui {

class QFGUI_DECL_EXPORT DialogButtonBox : public QDialogButtonBox
{
	Q_OBJECT
public:
	explicit DialogButtonBox(QWidget * parent = 0);
	explicit DialogButtonBox(StandardButtons buttons, QWidget *parent = nullptr);
};

}}

#endif
