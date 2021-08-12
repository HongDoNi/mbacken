#include "Logger.h"



void Logger::Log(const string &msg, const string &logFilePath, const string &objName)
{
	if(msg.empty())
	{
		return;
	}
	string logPath;
	if(logFilePath.empty())
	{
		logPath = DEFAULT_LOG_FILE_PATH;
	}
	else
	{
		logPath = logFilePath;
	}

	if(!objName.empty())
	{
		logPath += "_" + objName;
	}

	b2be::utils::File* logFile = new b2be::utils::File(logPath, b2be::utils::File::MODE_APPEND);
	if(logFile == NULL || logFile->IsError())
	{
		delete logFile;
		return;
	}

	logFile->Write(lvki::DateTime::NowString("%Y-%m-%d %H:%M:%S","GMT") + " - " + msg + "\n");
	logFile->Close();
	delete logFile;
}
