#include "Constants.h"
#include "packetSegment/PacketSegmentContext.h"
#include "ProtocolTemplate.h"
#include "CodeConfig.h"
#include "Packet.h"
#include "ControlParameters.h"
#include "wrap/WrapperChain.h"

class Encoder : public Constants
{
    
    
public:
    Encoder();
    int encode(Packet in, unsigned char* out,  int* nLen, ControlParameters* controlparas);

private:  
    WrapperChain wrapperChain;
   
    
    
int bcdToInt(int bcdValue){
    return (0x0f & bcdValue)+(0x0f & bcdValue>>4)*10;
 }

int intToBcd(int intValue){
    return (intValue/10)*16 + intValue%10;
}

};
