#include "HeadTailUnwrapper.h"

int HeadTailUnwrapper::HEAD_LENGTH = 6;
int HeadTailUnwrapper::TAIL_LENGTH = 2;

void HeadTailUnwrapper::decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext)
{
      
        if(nLen<6){ //报文长度小于6
            
            return;
        }

        unsigned char headArray[6]; 
        for(int i = 0; i<HEAD_LENGTH ; i++)
            headArray[i] = in[i];

        Head head=(Head)packetSegmentContext->getSegment(SegmentEnum::HEAD);
        Tail tail=(Tail)packetSegmentContext->getSegment(SegmentEnum::TAIL);

        if(headArray[0]!=headArray[5] || !(headArray[0]==0x68 || headArray[0]==0x69)  // "报文头校验错误"
                || headArray[1]!=headArray[3] || headArray[2]!=headArray[4])
        {
            return;
            
        }

        head.setVersion(headArray[1] & 0x03);
        head.setLength(((headArray[1] & 0xff)>>2)+((headArray[2] & 0xff) << 6));
        
        if(nLen != head.getLength() + 8 )  // 报文长度和实际长度不匹配
        {
            return ;
            
        }

        unsigned char CS;
        CS = in[nLen -2];

        unsigned char checkSum=0;
        for(int i=HEAD_LENGTH; i< HEAD_LENGTH + head.getLength() -2; i++) {
            checkSum |= (in[i] & 0xFF);
        }
        
        tail.setCheckSum(CS);

        if(checkSum!=CS){   //校验值错误
            return;            
        }

        head.setDecodeCompleted();
        tail.setDecodeCompleted();
        
        if(next!=null){
            next.decode(in,nLen,packetSegmentContext);
        }
}
