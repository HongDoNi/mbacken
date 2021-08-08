
#ifndef SERVICE_IMPL_H
#define SERVICE_IMPL_H

#include "ServiceBase.h"

#include <string>
#include <iostream>

using namespace std;

typedef enum device
{
	UNKNOWN = 0,
	CONCENTRATOR_1,   
	CONCENTRATOR_2,
	COLLECTOR,
	POWERMETER_1,
	POWERMETER_3,
	NETWORK
} DEVICE;

typedef enum devicefault
{
	DF00 = 0,
	DF01,
	DF02,
	DF03,
	DF04,
	DF05,
	DF06,
	DF07,
	DF08,
	DF09,
	DF10,
	DF11,
	DF12,
	DF13,
	DF14
} DEVICEFAULTID;

typedef struct devicefaultinfo 
{
    char GUID[15];
	DEVICE device;
	DEVICEFAULTID deviceFaultID;
	char deviceAssetID[12];
	char deviceType[16];
	char deviceCoreChipID[15];
	char deviceInstallAddr1[255];
	char deviceInstallAddr2[255];
	int deviceNoInGroup;
	char deviceFaultDate[8];
	char deviceFaultTime[8];
	char deviceProtocol[10];
	char deviceProtocolVersion[5];
} DFI;

typedef struct devicefaultpocess
{

	char deviceFaultInfoGUID[15];
	DEVICE device;
	DEVICEFAULTID deviceFaultID;
	char deviceFaultCause[255];
	char deviceFaultProcess[30];
	char deviceFaultProcessDate[8];
	bool reportToSupllier;
	char supplierFeedback[255];
}DFP;


class ServiceImpl : public ServiceBase
{
	public:
		ServiceImpl();
		virtual ~ServiceImpl();
		
		void Init();
		void Run();		
		void Stop();
		
		
	private:
		void RunProcessModel();
			
		void ProcessMessage(b2be::utils::Database* database);
//		void PostProcessMessage(b2be::utils::Database* database);
//		void PreProcessMessage(b2be::utils::Database* database);
        bool PostProcessMessage(b2be::utils::Database* database);
        bool PreProcessMessage(b2be::utils::Database* database);

		DFI dfi;
		DFP dfp;
				
};

#endif
