#include "Head.h"


Head::Head(SegmentEnum segmentID):Segment(segmentID)
{
    reset(); 

}

void Head::reset() 
{
    length=0;
    version=0;
    buffer.clear();
}