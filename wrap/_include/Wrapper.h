#include "PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"


class Wrapper 
{
   
    public:

    Wrapper();
    ~Wrapper();
    
    virtual void encode(Packet in,PacketSegmentContext* packetSegmentContext)=0;
    void setNext(Wrapper* wrapper);
   
    protected:
     Wrapper* next;
};
