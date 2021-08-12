#include "Control.h"


Control::Control(SegmentEnum segmentID) : Segment(segmentID);
{
    reset();
}

Control::~Control()
{
     
}   


void Control::reset()
{
        functionCode=0;
        fcbOrAcd=0;
        prm=0;
        dir=0;
        admin_code=0;
        concentrate_no=0;
        isGroup=0;
        msa=0;
        afn=0;
        seq=0;
        isNeedConfirm=0;
        fin=0;
        fir=0;
        tpV=0;
        isDecoded =  false;
  
}
    