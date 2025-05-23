#ifndef QF_CORE_STACKTRACE_H
#define QF_CORE_STACKTRACE_H

#include "coreglobal.h"

#include <QStringList>

namespace qf {
namespace core {

/// Helper class generating the current stack trace
/**
 Currently only Linux is supported.:((
*/
class QFCORE_DECL_EXPORT StackTrace
{
public:
	static QStringList stackTrace();
};

}
}

#endif // QF_CORE_STACKTRACE_H
