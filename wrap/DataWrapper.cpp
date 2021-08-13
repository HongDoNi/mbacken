#include "DataWrapper.h"

string DataWrapper:: MULTIPLECOMMAND = "multipleCommand";

DataWrapper::DataWrapper()
{
}

DataWrapper::~DataWrapper()
{   
}


void DataWrapper::encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas) 
{
    Data* dataSeg    = (Data*)packetSegmentContext->getSegment(SegmentEnum::DATA);
    //Control* control = (Control*)packetSegmentContext->getSegment(SegmentEnum::CONTROL);

    int afn = controlparas->getafn(); 
    int fn  = controlparas->getfn();
    int pn  = controlparas->getpn();
    int pfc = controlparas->getpfc();
    int iceday   =  controlparas->geticeday();
    int icemonth =  controlparas->geticemonth();
    int iceyear  =  controlparas->geticeyear();
    int second   = 1;
    int minute   = 1;
    int hour     = 1;
  
    dataSeg->setAFN(afn);     
    dataSeg->setFn(fn);
    dataSeg->setPn(pn);
    
    int pos = 4;
    int nLen = 4;   
    unsigned char buffer[4096];
    unsigned char DA1;
    unsigned char DA2;
    unsigned char DT1;
    unsigned char DT2;

    DA1 = (unsigned char)((fn%256)&0xff);
    buffer[0] = (DA1/10)*16 + DA1%10;

    DA2 = (unsigned char)((fn>>8)&0xff);
    buffer[1] = (DA2/10)*16 + DA2%10;

    DT1 = (unsigned char)((pn%256)&0xff);
    buffer[2] =  (DT1/10)*16 + DT1%10;
   
    DT2 = (unsigned char)((pn>>8)&0xff);
    buffer[3] = (DT2/10)*16 + DT2%10;


    switch(afn)
    {
    
     case 0x00: //确认∕否认
         if(fn ==1 || fn ==2)
             nLen = 4;
         break;
    case 0x01: //复位
        nLen = 4;
        break;
    case 0x02:  //链路接口检测
        if(fn ==3)  //心跳帧
        {
            buffer[pos]   = (unsigned char)(second & 0xff);   //秒
            buffer[pos+1] = (unsigned char)(minute & 0xff);   //分
            buffer[pos+2] = (unsigned char)(hour & 0xff);     //时
            buffer[pos+3] = (unsigned char)(iceday  & 0xff);  //日
            buffer[pos+4] = (unsigned char)(icemonth & 0xff); //月
            buffer[pos+5] = (unsigned char)(iceyear  & 0xff); //年
            nLen = 10;
        }
        break;
    case 0x04: //设置参数
    case 0x05: //控制命令
        buffer[pos]   = (unsigned char)(iceday & 0xff);   //日
        buffer[pos+1] = (unsigned char)(icemonth & 0xff); //月
        buffer[pos+2] = (unsigned char)(iceyear  & 0xff); //年
        nLen = 7;
        break;
    case 0x09: //请求终端配置信息
    case 0x0A: //查询参数
        nLen = 4;
        break;
    case 0x0B: // 请求任务数据
         if(fn == 1)
            nLen = 4;
        else
        {
            buffer[pos]   = (unsigned char)(minute & 0xff);   //分
            buffer[pos+1] = (unsigned char)(hour & 0xff);     //时
            buffer[pos+2] = (unsigned char)(iceday  & 0xff);  //日
            buffer[pos+3] = (unsigned char)(icemonth & 0xff); //月
            buffer[pos+4] = (unsigned char)(iceyear  & 0xff); //年
            nLen = 9;
        }
        break;            
    case 0x0C: //请求 1 类数据（实时数据）
        nLen = 4;
        break;
    case 0x0D: //请求2类数据：日冻结和月冻结数据            
        if((fn >=1 && fn <=4) || (fn >=27 && fn <=31)) // 日冻结数据
        {
            buffer[pos]   = (unsigned char)(iceday & 0xff);   //日
            buffer[pos+1] = (unsigned char)(icemonth & 0xff); //月
            buffer[pos+2] = (unsigned char)(iceyear  & 0xff); //年
            nLen = 7;
        }
        else if((fn >=17 && fn <=20)) //月冻结数据
        {
            buffer[pos]   = (unsigned char)(icemonth & 0xff); //月
            buffer[pos+1] = (unsigned char)(iceyear  & 0xff); //年
            nLen = 6;
        }          
        break;
    case 0x0E: //请求3类数据：故障数据       
        buffer[pos]   = (unsigned char)0x00;
        buffer[pos+1] = (unsigned char)0x29; 
        nLen = 6;   
        break;   
     }       
    
    dataSeg->setBuffer(buffer, nLen);

    if(next)
    {
        next->encode(in,packetSegmentContext,controlparas);
    }

}

