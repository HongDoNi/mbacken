
#include "Head.h"
#include "Tail.h"
#include "Data.h"
#include "Control.h"
#include "Auxiliary.h"
#include "PacketSegmentContext.h"


PacketSegmentContext::PacketSegmentContext()
{
    packetSegmentMap[SegmentEnum::HEAD]      = new Head(SegmentEnum::HEAD);
    packetSegmentMap[SegmentEnum::CONTROL]   = new Control(SegmentEnum::CONTROL);
    packetSegmentMap[SegmentEnum::DATA]      = new Data(SegmentEnum::DATA);
    packetSegmentMap[SegmentEnum::AUXILIARY] = new Auxiliary(SegmentEnum::AUXILIARY);
    packetSegmentMap[SegmentEnum::TAIL]      = new Tail(SegmentEnum::TAIL);
}

PacketSegmentContext::~PacketSegmentContext()
{
    reset();
}

void PacketSegmentContext::reset()
{
    Head* phead = packetSegmentMap[SegmentEnum::HEAD];
    if(phead)
        delete phead;

    Control* pControl =  packetSegmentMap[SegmentEnum::CONTROL];
    if(pControl)
        delete pControl;
    
    Data* pdata  = packetSegmentMap[SegmentEnum::DATA];
    if(pdata)
        delete pdata;

    Auxiliary* pAux  = packetSegmentMap[SegmentEnum::AUXILIARY];
    if(pAux)
        delete pAux;
    
    Tail* ptail = packetSegmentMap[SegmentEnum::TAIL];
    if(ptail)
        delete ptail;
        
    packetSegmentMap.clear();
}