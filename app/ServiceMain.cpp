
/**
 * common main() function used for all service framework types
 * not copied into source folder like V2 translations - makefile can just point to where this is
 */

#include <signal.h>


//use g++ compiler option -I/home/developer/include/cpp/ to prevent the need for full include paths in the code
//#include "utils/ProcessLock.h"
//#include "phoenix/ServiceInstance.h"

#include "ServiceImpl.h" //this is the actual source for the service we are compiling. Using "-I/service/source/path" will ensure the .h file will be found


ServiceImpl* service = NULL;
// b2be::utils::Database* serviceb = NULL;

void Usage(string executableName)
{
    cout << "Usage: " << executableName << " ARGUMENT1 ARGUMENT2 ARGUMENT3 ..." << endl << endl;
    cout << "Arguments for RESIDENT or SYSTEM service: " << endl;
    cout << "  --service-instance-id XXXX  Service Instance ID" << endl;
    cout << "  --service-id XXXX           Service ID" << endl;
    cout << "  --server-id XXXX            Server ID" << endl;
    cout << "  --beanstalk-type XXXX       Beanstalk Type" << endl;
    cout << "  --daemon                    (optional) Run as a daemon (ie. in background)" << endl;
    cout << "  --verbose                   (optional) Enable verbose logging" << endl << endl;
    cout << "Arguments for SCHEDULED service: " << endl;
    cout << "  --service-id XXXX           Service ID" << endl;
    cout << "  --server-id XXXX            Server ID" << endl;
    cout << "  --service-config-id XXXX    (optional) Service Config ID" << endl;
    cout << "  --verbose                   (optional) Enable verbose logging" << endl;
}

//handler for SIGTERM so the service can shut down nicely when issued the kill command
void HandleTermSignal(int param)
{
    if (service != NULL)
        service->StopBase();
}

//handler for SIGINT so the service can shut down nicely when ctrl+c pressed at command line
void HandleIntSignal(int param)
{
    if (service != NULL)
        service->StopBase();
}

//handler for SIGUSR1 so the service can toggle verbose mode
void HandleUsr1Signal(int param)
{
    if (service != NULL)
        service->ToggleVerbose();
}

//using namespace b2be::utils;
using namespace lvki;

int main (int argc, char** argv)
{
    string executableName = argv[0];

    bool isScheduledService = false;
    unsigned int serviceInstanceID = 0;
    unsigned int serviceID = 0;
    unsigned int serviceConfigID = 0;
    unsigned int serverID = 0;

    //string beanstalkTypeName = "";
    //ServiceInstance::BeanstalkType beanstalkType = ServiceInstance::TYPE_NONE;

    //lock the process so only one copy can run
    string unique = b2be::utils::String::ToString(serverID) + "_" + b2be::utils::String::ToString(serviceID) + "_" + b2be::utils::String::ToString(serviceInstanceID) + "_" + b2be::utils::String::ToString(serviceConfigID);
    ///ProcessLock* processLock = new ProcessLock(argv[0], unique);

    //if (!processLock->Acquire())
    //{
    //	cerr << "Process already running: " << executableName << endl;
    //	delete processLock;
    //	return EXIT_FAILURE;
    //}

    //perform the required signal handler setup (has to be done differently for Scheduled VS Resident or System services)
     
    if(isScheduledService)
    {
        signal(SIGTERM, HandleTermSignal);
        signal(SIGUSR1, HandleUsr1Signal);
    }
    else
    {
        signal(SIGTERM, HandleTermSignal);
        signal(SIGINT, HandleIntSignal);
        signal(SIGUSR1, HandleUsr1Signal);
    }


    export_data_to_test_table();
    

    //create service instance object
    //ServiceInstance* instance = new ServiceInstance(serviceInstanceID, serviceID, serverID, beanstalkType);

    //create service and tell service to Init() - loads settings, connects to DB's etc. base calls implementation specific version automatically
    // service = new ServiceImpl();
    // service->InitBase();

    //work out what the init error file path is
    // string initErrorFilePath = "";

//     if (service->IsError())
//     {
//         b2be::utils::File* initErrorFile = new b2be::utils::File(initErrorFilePath, b2be::utils::File::MODE_WRITE);
//         if (initErrorFile->IsError())
//         {
//             //all we can do is cout here, as we can't even open the init error file to write to!
//             cerr << "Init error has occurred (" + service->GetInitErrorMessage() + ") but cannot write it to file - " + initErrorFile->GetErrorMessage() << endl;
//         }
//         else
//         {
//             string initErrorMessage = service->GetInitErrorMessage();
//             initErrorFile->Write(initErrorMessage);
//             initErrorFile->Close();
//         }
//         delete initErrorFile;
//     }
//     else
//     {
//         //delete any existing init error file (which may or may not exist)
//         b2be::utils::File::Delete(initErrorFilePath);

//         //useful for testing
// //        if (verbose)
// //            service->ToggleVerbose();

//         // tell service to Run() - calls implemenation specific version automatically
//         service->RunBase();
//     }

    //delete the service
    // delete service;

//    processLock->Release();
    
    service = new ServiceImpl();
    
    service -> Init();
    service -> Run();
    service -> Stop();

    delete service;
    
    // b2be::utils::DBSettings mydbs("localhost", "root", "admin", 3306, false);
    // const string mydbname("test");
    // serviceb = new b2be::utils::Database(&mydbs, mydbname);
    // serviceb -> Connect();
    // if ( serviceb->IsError() ) {
    //     std::cout << "fail" << std::endl;
    // }
    // else {
    //     std::cout << "success" << std::endl;
    // }


    cout << "Service Shutdown Complete (" << executableName  << ")" << endl;

    return EXIT_SUCCESS;
}
