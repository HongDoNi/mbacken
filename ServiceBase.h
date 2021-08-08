#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <string>
#include <sstream>
#include <iostream>
#include <map>

#include "Database.h"
#include "DateTime.h"
#include "File.h"
#include "Logger.h"

#define INIT_ERROR_PERSIST_PATH "errors/init/"
#define SYSTEM_ERROR_PERSIST_PATH "errors/system/"
#define JOB_PROCESSING_PERSIST_PATH "jobs/processing/"
#define JOB_TOPUT_PERSIST_PATH "jobs/toput/"
#define VERBOSE_LOG_PERSIST_PATH "verboselogs/"

using namespace std;

class ServiceBase
{
	public:
		enum ErrorType {ERRORTYPE_INIT, ERRORTYPE_SYSTEM, ERRORTYPE_PROCESSING};		
		
		ServiceBase();
		virtual ~ServiceBase();		
				
		void InitBase();
		void RunBase();
		void StopBase();

		void setProgramName(const string sProgramName);
		
		
		virtual void Run()  = 0;		
		virtual void Stop() = 0;		
		
		void ToggleVerbose();
		
//		Settings* GetSettings();
        b2be::utils::DBSettings* GetSettings();


    bool IsError() const;
		ErrorType GetErrorType() const;
		string GetInitErrorMessage() const;
		virtual void ResetError();

	protected:	
		b2be::utils::Database* GetLocalDatabase();
		//Database* GetDatabaseByDataCentreID(unsigned int dcid);
		
		
		string PersistJob(string jobid, string data);
		string ErrorPutJob(string jobid, string jobpath);

		void RaiseInitError(string errorMessage);
		void RaiseSystemError(string errorMessage);
		void HandleSystemError();
		void HandleSystemError(string jobID, string persistedJobFilePath);
		
		void VerboseLog(const string &msg);
		
		b2be::utils::File* CreateTempFile();
		
		string MakeBackupPath(unsigned int serviceID, unsigned long messageID);

		
		bool _IsError;
		ErrorType _ErrorType;
		string _InitErrorMessage;
		string _SystemErrorMessage;
		string _VerboseLogFilePath;
		string _ErrorDateTime;
		string _ExecutableName; // zy add
	
		// ServiceInstance *_ServiceInstance;	
		//unsigned int _ServiceConfigID;	//ONLY for use by Scheduled services (for all other service types it'll be 0)
		//Settings *_PhoenixSettings;
		//Connection* _PhoenixConnection;
		//ServiceConfig* _ServiceConfig;
		
		bool _StopCalled;	
		bool _Verbose;
		bool _Daemon;

	private:
		// void ConnectDatabase();
		bool ConnectDatabase();  // zy changed
		//void ConnectPhoenix();
		//void LoadServiceAttributes();
		b2be::utils::Database* _database;
		//map<unsigned int, Database*> _Databases;
		//map<string, string> _ServiceAttributes;
};

#endif
