#include "HeadTailWrapper.h"

  
void HeadTailWrapper::encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas) 
{
    Head* head=(Head*)packetSegmentContext->getSegment(SegmentEnum::HEAD);
    Tail* tail=(Tail*)packetSegmentContext->getSegment(SegmentEnum::TAIL);
    Control* control =(Control*)packetSegmentContext->getSegment(SegmentEnum::CONTROL);
    Data* data =(Data*)packetSegmentContext->getSegment(SegmentEnum::DATA);
    Auxiliary* auxiliary = (Auxiliary*)packetSegmentContext->getSegment(SegmentEnum::AUXILIARY);

    unsigned char checkSum=0;
    int length = 0;
    int cLen   = 0;
    int dLen   = 0;
    unsigned char cbuffer[10];
    unsigned char dbuffer[4096];
    unsigned char abuffer[22];    
    unsigned char* buff;

    control->getBuffer(cbuffer, &cLen); 
    data->getBuffer(dbuffer, &dLen);
    auxiliary->getBuffer(abuffer, &aLen);

    length =  cLen + dLen + aLen;
    
    buff = new unsigned char [length];    
    
    for (int i = 0; i<cLen; i++)
        buff[i] = cbuffer[i];
    
    for (int j = 0; j<dLen; j++)
        buff[cLen+j] = dbuffer[j];

    for (int k = 0; k<aLen; k++)
        buff[cLen+dLen+k] = abuffer[k];
    
    
    for (int k = 0; k<length; k++)
        checkSum |= buff[k];    
    
    tail->setCheckSum(checkSum);
    head->setLength(length);

    delete []buff;
        
    //标记头
    unsigned char headTag=0x68;
    int afn = control->getAfn();
    if(afn==0x04 || afn==0x05) {
        headTag=0x69;
    }
     
    head->setHeadtag(headTag);
   
    if(next){
        next->encode(in,packetSegmentContext);
    }
}