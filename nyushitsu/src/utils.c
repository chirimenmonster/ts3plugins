#include <stdio.h>
#include <stdarg.h>

#include "ts3_functions.h"
#include "nyushitsu.h"

extern struct TS3Functions ts3Functions;

void logMessage(const char *format, ...) {
	char buf[1024];

	va_list va;
	va_start(va, format);
	vsnprintf(buf, sizeof(buf), format, va);
	va_end(va);

	ts3Functions.logMessage(buf, LogLevel_INFO, PLUGIN_NAME, 0);
}
