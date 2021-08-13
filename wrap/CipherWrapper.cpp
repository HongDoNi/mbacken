#include "CipherWrapper.h"

CipherWrapper::CipherWrapper()
{
}

CipherWrapper::~CipherWrapper()
{
}

void encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas)
{
    Control* control=(Control*)packetSegmentContext->getSegment(SegmentEnum.control);
    Data* dataSeg=(Data*)packetSegmentContext->getSegment(SegmentEnum.data);

    int afn = control->getAfn();

    if(afn==0x04 || afn==0x05)
    {
        ;
        //list<byte[]> body = dataSeg.getBuffer();
        //加密
        //list<byte[]> crypted = body;
        //dataSeg.getBuffer().clear();
        //dataSeg.getBuffer().addAll(crypted);
    }

    if(next){
        next->encode(in,packetSegmentContext,controlparas);
    }
}