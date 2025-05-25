#include "stacktrace.h"

#include <QStringList>

#if defined Q_CC_GNU && !defined Q_CC_MINGW && !defined ANDROID
#include <cstdlib>
#include <execinfo.h>
#endif

using namespace qf::core;


QStringList StackTrace::stackTrace()
{
#if defined Q_CC_GNU && !defined Q_CC_MINGW
	const int max_frames = 100;
	void* addrlist[max_frames]; // NOLINT(modernize-avoid-c-arrays)

	// Retrieve current stack addresses
	int addrlen = backtrace(addrlist, max_frames);

	if (addrlen == 0) {
		return {};
	}

	// Create readable strings for each frame
	auto symbollist = backtrace_symbols(addrlist, addrlen);

	QStringList sl_ret;
	for (int i = 0; i < addrlen; i++) {
		sl_ret << symbollist[i];
	}

	free(symbollist); // NOLINT(bugprone-multi-level-implicit-pointer-conversion,cppcoreguidelines-no-malloc,hicpp-no-malloc)
	return sl_ret;
#else
	return {};
#endif
}

