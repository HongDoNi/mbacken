#include "Wrapper.h"

class HeadTailWrapper : public Wrapper
{

    
public:
    HeadTailWrapper();
    ~HeadTailWrapper();

    void encode(Packet in, PacketSegmentContext* packetSegmentContext); 
};
