#include "Wrapper.h"
#include "Packet.h"
#include "PacketSegmentContext.h"
#include "ControlParameters.h"

class AuxWrapper : public Wrapper
{

    public:
    AuxWrapper();
    ~AuxWrapper();
    void encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas);
};
