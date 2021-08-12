#include "Tail.h"

Tail::Tail(SegmentEnum segmentID):Segment(segmentID)
{
    reset();

}

void Tail::reset() 
{
    checkSum=0;
    buffer.clear();
}
