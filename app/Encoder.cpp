#include "Encoder.h"

Encoder::Encoder()
{       
    wrapperChain.add(new ControlWrapper());
    wrapperChain.add(new DataWrapper());
    wrapperChain.add(new CipherWrapper());
    wrapperChain.add(new AuxWrapper());
    wrapperChain.add(new HeadTailWrapper());
}

int Encoder::encode(Packet in, unsigned char* out, int* nLen,  ControlParameters* controlparas)  
{
      
        PacketSegmentContext packetSegmentContext;  

        //编码操作
        wrapperChain.encode(in, &packetSegmentContext, controlparas);

        //参数设置
        Head* head=(Head*)packetSegmentContext.getSegment(SegmentEnum::HEAD);
        Tail* tail=(Tail*)packetSegmentContext.getSegment(SegmentEnum::TAIL);

        //规约版本设置
        head->setVersion(0x01); 
        Auxiliary* auxiliary=(Auxiliary*)packetSegmentContext.getSegment(SegmentEnum::AUXILIARY);
        Control* control=(Control*)packetSegmentContext.getSegment(SegmentEnum::CONTROL);         

        //帧序号设置
        control->setSeq(pfc);

        int length = head->getLength() + 8;
        out = new unsigned char[length];
        *nLen = length;  
        
        fillHeadSegment(out, length, head);
        fillControlSegment(out, length, control);
        fillDataSegment(out, length, data);
        fillAuxiliarySegment(out, length, auxiliary);
           
        out[length-2] = (tail->getCheckSum()/10)*16 + tail->getCheckSum()%10;
        out[length-1] = 0x16;

        //返回控制信息
        int result=1;
        if(control->getIsNeedConfirm()==1){
            result=2;
        }

        //清空编码结果
        packetSegmentContext.reset();
        return result;
    
}



int Encoder::fillHeadSegment(unsigned char* out, int nLen, Head* head, Tail* tail)
{
    out[0] = head->getheadTag();
    out[5] = head->getheadTag();

    unsigned char high =  (unsigned char)((head->getLength() <<2)/256);  
    unsigned char low  =  (unsigned char)((head->getLength() <<2)%256)+0x01);

    out[1] = (low/10)*16 + low%10;
    out[2] = (high/10)*16 + high%10;
    
    out[3] = (low/10)*16 + low%10;
    out[4] = (high/10)*16 + high%10;

 }


int Encoder::fillControlSegment(unsigned char* out, int nLen, Control* control)
{
    int nLen = 0;
    int pos  = 6;
    unsigned char value[8];
    control->getBuffer(value, &nLen);

    for(int i = 0; i<nLen; i++)
    {
        out[pos+i] = value[i];
    }
   
}

int Encoder::fillDataSegment(unsigned char* out, int nLen, Data* data)
{
    
    int nlength = 0;
    int pos = 15;
    unsigned char* value = new unsigned char [nLen];
    value = data->getBuffer(&value, &nlength);

    for(int i = 0; i<nlength; i++)
    {
        out[pos+i] = value[i];
    }
}

int Encoder::fillAuxiliarySegment(unsigned char* out, int nLen, Auxiliary* auxiliary)
{
   
    if(auxiliary->isHaveTp())
    {       
        out[nLen-3] = auxiliary->getTimeout();

        unsigned char Tp[4];
        auxiliary->getTp(Tp);

        out[nLen-4] = Tp[0];
        out[nLen-5] = Tp[1];
        out[nLen-6] = Tp[2];
        out[nLen-7] = Tp[3];
        out[nLen-8]= auxiliary->getPfc();
    }

    if(auxiliary->isHavePW())   
    {
        unsigned char pw[16];
        auxiliary->getPW(pw);

        for(int i = 0; i<16; i++)
            out[nLen-9-15 - i] = pw[i];
    }
   
}        