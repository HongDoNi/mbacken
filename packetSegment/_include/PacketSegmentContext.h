
#include "Segment.h"
#include <map>

class PacketSegmentContext 
{

public:
    PacketSegmentContext();
    ~PacketSegmentContext();

    Segment* getSegment(SegmentEnum segmentEnum)
    {
        return packetSegmentMap[segmentEnum];
    }

    void reset();
   
    private:
    map<SegmentEnum, Segment*> packetSegmentMap;

};
