#include "ServiceImpl.h"


ServiceImpl::ServiceImpl()
{
	
}


ServiceImpl::~ServiceImpl()
{
	
}


void ServiceImpl::Init()
{
	// To Do
}


void ServiceImpl::Run()
{
	RunProcessModel();
}


void ServiceImpl::RunProcessModel()
{
	while (!_StopCalled)
	{
		
		b2be::utils::Database* database = GetLocalDatabase();
				
		unsigned long messageActionID = 0;
		if(!PreProcessMessage(database))
		{
			continue;			
		}
		
		database->BeginTransaction();
		ProcessMessage(database);
		
		if(_IsError)
		{
			database->RollbackTransaction();
			PostProcessMessage(database);
			continue;
		}		
	
		database->CommitTransaction();		

		if (_Verbose)
			VerboseLog("Data has been successfully stored.");				
	
		if(!PostProcessMessage(database))
			continue;				
				
	}
}


void ServiceImpl::Stop()
{
	
}

//void ServiceImpl::PreProcessMessage(b2be::utils::Database* database)
bool ServiceImpl::PreProcessMessage(b2be::utils::Database* database)
{
    return false;
}

//void ServiceImpl::PostProcessMessage(b2be::utils::Database* database)
bool ServiceImpl::PostProcessMessage(b2be::utils::Database* database)
{
    return false;

}


void ServiceImpl::ProcessMessage(b2be::utils::Database* database)
{	

//	string sqlstat = "INSERT INTO DeviceFaultInfo (DeviceFaultID, DeviceAssetID, DeviceType, DeviceCoreChipID, DeviceInstallAddr1, DeviceInstallAddr2, DeviceNoInGroup, DeviceFaultDate, DeviceFaultTime, DeviceProtocol, DeviceProtocolVersion) ";
//	sqlstat.append("VALUES (\"");
//	sqlstat.append(deviceFaultID);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceAssetID);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceType);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceCoreChipID);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceInstallAddr1);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceInstallAddr2);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceNoInGroup);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceFaultDate);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceFaultTime);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceProtocol);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceProtocolVersion);
//	sqlstat.append("\" )");
//
//    b2be::utils::DBQuery* query = database->CreateQuery(sqlstat);
//    b2be::utils::DBQueryResult* result = query->Execute();
//
//	if(query->IsError())
//	{
//		RaiseSystemError(query->GetErrorMessage());
//
//		delete query;
//		if (result)
//			delete result;
//	}
//
//	delete result;
//	delete query;
//	result = NULL;
//	query  = NULL;
//
//	sqlstat = "INSERT INTO DeviceFaultProcess (DeviceFaultInfoID, DeviceFaultCause, DeviceFaultProcess, DeviceFaultProcessDate, ReportToSupllier, SupplierFeedback) ";
//	sqlstat.append("VALUES (\"");
//	sqlstat.append(deviceFaultID);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceAssetID);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceType);
//	sqlstat.append("\", \"");
//	sqlstat.append(deviceCoreChipID);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceInstallAddr1);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceInstallAddr2);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceNoInGroup);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceFaultDate);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceFaultTime);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceProtocol);
//	sqlstat.append("\", \"");
//	sqlstat.append(DeviceProtocolVersion);
//	sqlstat.append("\" )");
//
//	query = database->CreateQuery(sqlstat);
//	result = query->Execute();
//
//	if(query->IsError())
//	{
//		RaiseSystemError(query->GetErrorMessage());
//
//		delete query;
//		if (result)
//			delete result;
//
//	}
//
//	delete result;
//	delete query;
}

