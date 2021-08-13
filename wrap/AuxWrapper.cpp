#include "AuxWrapper.h"
AuxWrapper::AuxWrapper()
{

}

AuxWrapper::~AuxWrapper()
{
    
}



void AuxWrapper::encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas)
{
    Control* control =(Control*)packetSegmentContext->getSegment(SegmentEnum::CONTROL);
    Auxiliary* auxiliary =(Auxiliary*)packetSegmentContext->getSegment(SegmentEnum::AUXILIARY);

    unsigned char buff[22];
    int nLen = 0;
    int pos  = 0;

    if(control->getTpV()==1)
        auxiliary->setTp(true);

    int afn = control->getAfn();
    if(afn==0x01 || afn==0x04 || afn==0x05 || afn==0x0F)
    {          
        auxiliary->setPW(true);        
        
    }

    if(auxiliary->isHavePW())
    {
        unsigned char pw[16];
        for(int j=0; j<16; j++)
        {
            pw[j]   = 0x00;
            buff[j] = 0x00;
        }    

        auxiliary->setPW(pw, 16);
        pos = 16;
        nLen = 16;
    }
    
   
    if(auxiliary->isHaveTp())
    {
        auxiliary->setTimeout(5);
        buff[pos] = 0x05; 

        unsigned char Tp[4];
        Tp[0] = 45; 
        Tp[1] = 23;
        Tp[2] = 8;
        Tp[3] = 17;
        
        buff[pos+1] = 0x45;
        buff[pos+2] = 0x23;
        buff[pos+3] = 0x8;
        buff[pos+4] = 0x15;

        auxiliary->setTp(Tp, 4);
        auxiliary->setPfc(pfc);
        
        buff[pos+5] = pfc;
        nLen += 6;
    }

    auxiliary->setBuffer(buff, nLen);

    if(next){
        next->encode(in,packetSegmentContext);
    }
}

