#include "ServiceBase.h"
#include "Database.h"


ServiceBase::ServiceBase()
{
	_StopCalled = false;
	_Verbose = false;
	ResetError();
}


 
ServiceBase:: ~ ServiceBase()
{
	
	_database->Disconnect();
	delete _database;
	
}


void ServiceBase::InitBase()
{
	
	//init a random seed using the time as well as service instance id and server id just to reduce the chance of 2 servers initialising to the same seed
	unsigned int seed = (unsigned)time(0);
	srand(seed);
		
	
	ConnectDatabase();

	if(_IsError)
		return;	
	
	stringstream logFilePath;
	logFilePath << VERBOSE_LOG_PERSIST_PATH << _ExecutableName << ".log";
	_VerboseLogFilePath = logFilePath.str();
}


void ServiceBase::setProgramName(const string sProgramName)
{
	_ExecutableName = sProgramName ;
}


/**
 * Creates all required database connections but does not connect to them at this stage.
 */
bool ServiceBase::ConnectDatabase()
{
	b2be::utils::DBSettings dbs("localhost", "root", "admin", 3306, false);
	// dbs.readDatabaseInfo(DATABASE_CONFIG_FILE);
	const string dbname("test"); // zy add
	_database = new b2be::utils::Database(&dbs, dbname); // zy changed
	_database->Connect();

	if (_database->IsError())
	{
		RaiseSystemError("Unable to connect to database at " + _database->GetHost() + " : " + _database->GetErrorMessage());
		return false;
	}

	return true;	
}


b2be::utils::Database* ServiceBase::GetLocalDatabase()
{
	return _database;
}


/*
void ServiceBase::LoadServiceAttributes()
{
	Database* localDatabase = GetLocalDatabase();
	if(localDatabase == NULL)
	{
		RaiseInitError("Unable to get local database connection - GetLocalDatabase returned NULL");
		return;
	}
	
	//Now get the details from the ServiceAttribute table
	stringstream sql;
	sql.str("");
	sql << "SELECT `Key`, `Value` ";
	sql << "FROM " << _PhoenixSettings->GetPhoenixDatabaseName() << ".ServiceAttribute ";
	sql << "WHERE ServiceID = " << _ServiceInstance->GetServiceID();	
	DBQuery* query = localDatabase->CreateQuery(sql.str());
	DBQueryResult* result = query->Execute();
	if(query->IsError())
	{
		RaiseInitError("Unable to retrieve ServiceAttributes - " + query->GetErrorMessage());
		
		delete query;
		return;
	}
	
	while(result->IsMoreRows())
	{
		result->NextRow();
		_ServiceAttributes[result->GetColumnValueString("Key")] = result->GetColumnValueString("Value");
	}
	
	delete query;
	delete result;
}
*/



void ServiceBase::RunBase()
{
	Run();
}


void ServiceBase::StopBase()
{
	_StopCalled = true;
	Stop();
}


bool ServiceBase::IsError() const
{
	return _IsError;
}

ServiceBase::ErrorType ServiceBase::GetErrorType() const
{
	return _ErrorType;
}


string ServiceBase::GetInitErrorMessage() const
{
	return _InitErrorMessage;
}


void ServiceBase::ResetError()
{
	if(_Verbose)
		VerboseLog("Reset errors (ServiceBase)");

	_IsError = false;
	_ErrorType = ERRORTYPE_INIT;
	_InitErrorMessage = "";
	_SystemErrorMessage = "";
	_ErrorDateTime = "";
}


void ServiceBase::ToggleVerbose()
{
	_Verbose = !_Verbose;
}



void ServiceBase::VerboseLog(const string &msg)
{
	Logger::Log(msg, _VerboseLogFilePath);
	if(!_Daemon)
	{
		string logEntry = lvki::DateTime::NowString("%Y-%m-%d %H:%M:%S","GMT") + " - " + msg + "\n";
		cout << logEntry << endl;
	}
}


b2be::utils::File* ServiceBase::CreateTempFile()
{
	//  if(_ServiceInstance->GetBeanstalkType() == ServiceInstance::TYPE_REQUESTRESPONSE)
	// 	return new b2be::utils::File(lvki::DateTime::NowString("%Y%m%d%H%M%S", "GMT"), b2be::utils::File::MODE_CREATEUNIQUE);
	// else
	// 	return new b2be::utils::File(lvki::DateTime::NowString("%Y%m%d%H%M%S", "GMT"), b2be::utils::File::MODE_CREATEUNIQUE);
	return new b2be::utils::File(lvki::DateTime::NowString("%Y%m%d%H%M%S", "GMT"), b2be::utils::File::MODE_CREATEUNIQUE); // zy add
}


string ServiceBase::MakeBackupPath(unsigned int serviceID, unsigned long messageID)
{
	lvki::DateTime *now = lvki::DateTime::Now("GMT");
	stringstream destFilePath;
	destFilePath << now->ToString("/%Y/%m/%d/%H/");
	if (false == b2be::utils::File::MakeDirectory(destFilePath.str()))
		b2be::utils::File::MakeDirectory(destFilePath.str()); //try again - rare situation where MakeDirectory fails for no apparent reason
		
	destFilePath << now->ToString("%Y%m%d%H%M%S_") << messageID;
	delete now;
	
	return destFilePath.str();
}


string ServiceBase::PersistJob(string jobid, string data)
{
	stringstream filepath;
	filepath << JOB_PROCESSING_PERSIST_PATH << "_" << jobid ;
	
	b2be::utils::File *file = new b2be::utils::File(filepath.str(), b2be::utils::File::MODE_CREATEUNIQUE);
	
	if (file->IsError())
	{
		if(_Verbose)
			VerboseLog("Error persisting file: " + file->GetErrorMessage());
		delete file;
		return "";
	}
	
	file->Write(data);
	file->Close();
	
	if (file->IsError())
	{
		if(_Verbose)
			VerboseLog("Error persisting file: " + file->GetErrorMessage());
		delete file;
		return "";
	}
	
	string uniquefile = file->GetFilename();
	delete file;
	return uniquefile;
}



void ServiceBase::RaiseInitError(string errorMessage)
{
	_IsError = true;
	_ErrorType = ERRORTYPE_INIT;
	_InitErrorMessage = errorMessage;
	_ErrorDateTime = lvki::DateTime::NowString("%Y-%m-%d %H:%M:%S", "GMT");
	
	if(_Verbose)
		VerboseLog("Init error raised - " + errorMessage);
}


void ServiceBase::RaiseSystemError(string errorMessage)
{
	_IsError = true;
	_ErrorType = ERRORTYPE_SYSTEM;
	_SystemErrorMessage = errorMessage;
	_ErrorDateTime = lvki::DateTime::NowString("%Y-%m-%d %H:%M:%S", "GMT");

	if(_Verbose)
		VerboseLog("System error raised - " + errorMessage);
}

/*
void ServiceBase::LoadServiceConfig(Database* localDatabase, unsigned int serviceConfigID)
{
	delete _ServiceConfig;
	_ServiceConfig = NULL;

	//First, get the details from the ServiceConfig table
	stringstream sql;
	sql << "SELECT ID, Name, DataCentreID, ServiceID, LastModifierADUsername, LastModifiedDateTime, Enabled, Deleted ";
	sql << "FROM " << _PhoenixSettings->GetPhoenixServiceConfigDatabaseName() << ".ServiceConfig ";
	sql << "WHERE ID = " << serviceConfigID << " ";
	sql << "AND Enabled = \"Yes\" ";
	sql << "AND Deleted = \"No\"";	

	DBQuery* serviceConfigQuery = localDatabase->CreateQuery(sql.str());
	DBQueryResult* serviceConfigResult = serviceConfigQuery->Execute();
	if(serviceConfigQuery->IsError())
	{
		RaiseSystemError("Unable to retrieve ServiceConfig - " + serviceConfigQuery->GetErrorMessage());
		
		delete serviceConfigQuery;
		delete serviceConfigResult;
		return;
	}
	else if(serviceConfigResult->GetRowCount() == 0)
	{
		RaiseSystemError("Unable to retrieve ServiceConfig - got no result from database");
		
		delete serviceConfigQuery;
		delete serviceConfigResult;
		return;
	}
	
	//Now create the Service Config object using the details retrieved from the ServiceConfig table (above)
	serviceConfigResult->NextRow();
	
	DateTime* lastModifiedDateTime = new DateTime(serviceConfigResult->GetColumnValueString("LastModifiedDateTime"), "GMT");

	_ServiceConfig = new ServiceConfig(serviceConfigResult->GetColumnValueInt("ID"), serviceConfigResult->GetColumnValueString("Name"), serviceConfigResult->GetColumnValueInt("DataCentreID"), serviceConfigResult->GetColumnValueInt("ServiceID"), serviceConfigResult->GetColumnValueString("LastModifierADUsername"), lastModifiedDateTime, serviceConfigResult->GetColumnValueString("Enabled"), serviceConfigResult->GetColumnValueString("Deleted"));
	if(_ServiceConfig->IsError())
	{
		RaiseSystemError("Unable to create service config object  - " + _ServiceConfig->GetErrorMessage());
		
		delete lastModifiedDateTime;
		delete serviceConfigQuery;
		delete serviceConfigResult;	
		return;	
	}
	
	delete lastModifiedDateTime;
	delete serviceConfigQuery;
	delete serviceConfigResult;	
}
*/
