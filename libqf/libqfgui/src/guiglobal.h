#ifndef QFGUIGLOBAL_H
#define	QFGUIGLOBAL_H

#include <qglobal.h>

#if defined(QFGUI_BUILD_DLL)
//#warning "EXPORT"
#  define QFGUI_DECL_EXPORT Q_DECL_EXPORT
#else
//#warning "IMPORT"
#  define QFGUI_DECL_EXPORT Q_DECL_IMPORT
#endif

#endif // QFGUIGLOBAL_H
