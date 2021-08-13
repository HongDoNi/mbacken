#include "Wrapper.h"
#include "Packet.h"
#include "ControlParameters.h"
#include "PacketSegmentContext.h"
#include <list>

using namespace std;

class WrapperChain
{
   
    public:
    WrapperChain();
    ~WrapperChain();

    void add(Wrapper* wrapper);    
    void encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas);
       
     list<Wrapper*> wrappers;

};
