#include "ControlWrapper.h"

string ControlWrapper::MULTIPLECOMMAND = "multipleCommand";

ControlWrapper::ControlWrapper()
{
    
}


ControlWrapper::~ControlWrapper()
{  

}

void ControlWrapper::beforeEncode(Packet in, Control* control, Data* dataSeg, ControlParameters* controlparas)
{
     
    int afn = controlparas->getafn(); 
    int fn  = controlparas->getfn();
    int pn  = controlparas->getpn();

    long int admin_code;
    int concentrate_no;
    int Msa; 

    admin_code     = controlparas->getAdmin_code();
    concentrate_no = controlparas->getconcentrate_no();
    Msa            = controlparas->getMSA();

    control->setAdmin_code(admin_code);
    control->setconcentrate_no(concentrate_no);
    control->setMas(Msa);
    control->setAfn(afn);

    dataSeg->setFn(fn);
    dataSeg->setPn(pn);

    
    //设置功能码
    switch(afn)
    {
        case 0x00:
            control->setFunctionCode(0);
            break;
        case 0x01:
            control->setFunctionCode(1);
            break;
        case 0x02:
            control->setFunctionCode(9);
            break;
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x0f:
            control->setFunctionCode(10);
            break;
        case 0x13:
            control->setFunctionCode(4);
            break;
    
        default:
            control->setFunctionCode(11);
    }
 
    
    //设置广播地址
    if(control->getconcentrate_no() == 0xFFFF)
    {
        control->setIsGroup(1);
        //主站地址设置
        Msa  = (Msa << 1) | 0x01;
        control->setMsa(Msa); // 主站地址
    }
    else
    {
        control->setIsGroup(0);
        //主站地址设置
        Msa  = (Msa << 1) & 0xFE;
        control->setMsa(Msa); // 主站地址
    }
    

   //设置是否需要确认
    if(afn==0x01 || afn==0x04 || afn==0x05 || afn==0x0F)
    {
        control->setIsNeedConfirm(1);
        control->setFcbOrAcd(1);
    }
    else 
    {
        control->setIsNeedConfirm(0);
        control->setFcbOrAcd(0);
    }
 

    //启动站
    control->setPrm(1);
    //下行报文
    control->setDir(0);
    //报文是第一帧
    control->setFin(1);
    //报文是最后一帧
    control->setFir(1);

    //是否有时间标识
    if(afn == 0x01 || afn == 0x04 || afn == 0x05 || afn == 0x06 || afn == 0x09 || afn == 0x0A || afn == 0x0B || afn == 0x0C || fn == 0x0D || afn == 0x0E || afn == 0x0F)
        control->setTpV(true);
    else
        control->setTpV(false);   
   
}
 
void encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas)
{
    Control* control =(Control*)packetSegmentContext->getSegment(SegmentEnum::CONTROL);
    Data* dataSeg    =(Data)packetSegmentContext->getSegment(SegmentEnum::DATA);
    
    beforeEncode(in, control, dataSeg, controlparas);
    
    unsigned char buffer[8];
    int nLen = 8;

    //控制域功能码,帧计数有效位,fcb帧计数位,启动标志位,传输方向位 编码
    unsigned char c = (unsigned char) (control->getFunctionCode()+(control->getFcv()<<4)+(control->getFcbOrAcd()<<5)+(control->getPrm()<<6)+(control->getDir()<<7));
    buffer[0] =  (c/10)*16 + c%10;

    //地址编码
    c = (unsigned char)(control->getAdmin_code() & 0xff);
    buffer[1]  = (c/10)*16 + c%10;

    c = (unsigned char)((control->getAdmin_code()>>8) & 0xff);   
    buffer[2]  = (c/10)*16 + c%10;
    
    c = (unsigned char)(control->getconcentrate_no() & 0xFF);
    buffer[3]  = (c/10)*16 + c%10;
    
    c = (unsigned char)((control->getconcentrate_no()>>8) & 0xff);
    buffer[4]  = (c/10)*16 + c%10;

    //主站地址，
    c = (unsigned char)(control->getIsGroup()+(control->getMsa()<<1));
    buffer[5] =  (c/10)*16 + c%10;
    
    //afn编码
    c = (unsigned char)(control->getAfn()&0xFF);
    buffer[6] = (c/10)*16 + c%10;
       
    //帧序列，是否需确认，是否报文第一帧，是否报文最后一帧，附加信息是否有时间标签 编码
    c = (unsigned char)((control->getSeq()&0x0f)+(control->getIsNeedConfirm()<<4)+(control->getFin()<<5)+(control->getFir()<<6)+(control->getTpV()<<7));
    buffer[7] = (c/10)*16 + c%10;

    control->setBuffer(buffer, nLen);

    if(next){
        next->encode(in,packetSegmentContext);
    }

}

