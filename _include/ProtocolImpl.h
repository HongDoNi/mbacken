#include "Protocol.h"
#include "Decoder.h"
#include "Encoder.h"
#include "Packet.h"




class ProtocolImpl : public Protocol 
{

public:
    ProtocolImpl();
    ~ProtocolImpl();    
        
    int encode(Packet in, unsigned char* pout);   
    int decode(unsigned char in[], int nLen, Packet out); 
     
private:
    static  int SEQ_NOT_MATCH;
    static  int TIMEOUT_REACH;
    static  int HAVE_NEXT_FRAME;

    Decoder* decoder;
    Encoder* encoder;
    
    //0-idle,1-waitResponse
    int ProtocolStatus;
    string lastEncodedTerminalAddress;
    //上次编码的返回值
    int lastReturn=1;
    //帧序号记录器
    int pfc=0;
};
