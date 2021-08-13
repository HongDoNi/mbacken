//开始解码
        @Transit(from= ProtocalState.IDLE, to= ProtocalState.IDLE,
                on=Action.DECODE),
        //开始编码
        @Transit(from= ProtocalState.IDLE, to= ProtocalState.WAIT_RESPONSE,
                on=Action.ENCODE),
        //主动上传，忽略
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.WAIT_RESPONSE,
                on=Action.DECODE,when=InitiativeUploadCondition.class),
        //收到回复，没有多帧
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.IDLE,
                on=Action.DECODE,when=SingleEncodeCondition.class),
        //等待多帧解码
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.WAIT_NEXT_DECODE,
                on=Action.DECODE,when=MultipleDecodeCondition.class),
        //等待多帧解码
        @Transit(from= ProtocalState.WAIT_NEXT_DECODE, to= ProtocalState.WAIT_NEXT_DECODE,
                on=Action.DECODE,when=MultipleDecodeCondition.class),
        //完成最后一帧解码
        @Transit(from= ProtocalState.WAIT_NEXT_DECODE, to= ProtocalState.IDLE,
                on=Action.DECODE,when=SingleEncodeCondition.class),





/**
 * common main() function used for all service framework types
 * not copied into source folder like V2 translations - makefile can just point to where this is
 */

#include <signal.h>
#include "ServiceImpl.h"  

ServiceImpl* service = NULL;

int main (int argc, char** argv)
{
	string executableName = argv[0];

	//create service and tell service to Init() - loads settings, connects to DB's etc. base calls implementation specific version automatically
	service = new ServiceImpl(); 
	service->setProgramName(executableName);
	service->InitBase(); 
    service->RunBase();	
    
	delete service;
			
	cout << "Service Shutdown Complete (" << executableName  << ")" << endl;
	
	return EXIT_SUCCESS;
}
