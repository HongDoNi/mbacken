#include "Auxiliary.h"

Auxiliary::Auxiliary(SegmentEnum segmentID): Segment(segmentID)
{
    reset();
}

Auxiliary::~Auxiliary()
{

}

void Auxiliary::reset() 
{
    password="";
    ec1=0;
    ec2=0;
    pfc=0;
    timeout=0;
    Tp[0] = 0;
    Tp[1] = 0;
    Tp[2] = 0;
    Tp[3] = 0;
    for(int i=0; i<16; i++)
        PW[i] = 0x00;
    
}


void Auxiliary::setTp(unsigned char tp[])
{
    for(int i = 0; i < 4; i++ )
        this->Tp[i] = tp[i];

}

void Auxiliary::getTp(unsigned char tp[])
{
    for(int i = 0; i < 4; i++ )
        tp[i] = this->Tp[i];

}


void Auxiliary::getPW(unsigned char pw[])
{
    for(int i = 0; i < 16; i++ )
        pw[i] = this->PW[i];

}

void Auxiliary::setPW(unsigned char pw[], int nLen)
{

    for(int i = 0; i < nLen; i++ )
        this->PW[i] = pw[i];
}


bool Auxiliary::isHavePW()
{
    return  hasPW;  
}


bool Auxiliary::isHaveTp()
{
    return hasTp;
}

void Auxiliary::setTp(bool tp)
{
   
    hasTp = tp;
}

void Auxiliary::setPW(bool pw)
{
    hasPW = pw;

}
