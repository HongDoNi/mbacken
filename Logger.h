#ifndef LOGGER_H
#define LOGGER_H

#define DEFAULT_LOG_FILE_PATH	"/tmp/debug_log"

#include "DateTime.h"
#include "File.h"

using namespace std;

	
class Logger
{
	public:
		static void Log(const string &msg, const string &logFilePath = "", const string &objName = "");
};

#endif
