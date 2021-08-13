#include "Wrapper.h"

class CipherWrapper : public Wrapper
{
    public:
    void encode(Packet in, PacketSegmentContext packetSegmentContext,ControlParameters* controlparas);
    
}
