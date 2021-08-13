#include "CipherUnwrapper.h"


int CipherUnwrapper::CONTROL_LENGTH = 10;
int CipherUnwrapper::HEAD_LENGTH    = 6;

void CipherUnwrapper::decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext) 
{
    if(in[0]==0x69 || in[0]==0x68)
    {
        Control control=(Control)packetSegmentContext->getSegment(SegmentEnum::CONTROL);
        Head head=(Head)packetSegmentContext->getSegment(SegmentEnum::HEAD);

        //固定剩余数据长度
        int tailLeft=4;
        if(control.getFcbOrAcd()==1)
        {
            tailLeft+=2;
        }

        if(control.getTpV()==1)
        {
            tailLeft+=6;
        }
            
        int bodyLength = head.getLength()-tailLeft-4-CONTROL_LENGTH;

        unsigned char* headft  =new unsigned char[CONTROL_LENGTH + HEAD_LENGTH];
        unsigned char* body    =new unsigned char[bodyLength];
        unsigned char* tailft  =new unsigned char[tailLeft];
        
        in.get(headft);
        in.get(body);
        in.get(tailft);

     //解密
        byte[]  decrypted=body;

        ByteBuffer out=ByteBuffer.allocate(headft.length+decrypted.length+tailLeft);
        out.put(headft);
        out.put(decrypted);
        out.put(tailft);
        out.position(CONTROL_LENGTH+HEAD_LENGTH);

        if(next!=null)
        {
            next.decode(out, packetSegmentContext);
        }
    }
    else
    {
        if(next!=null)
        {
            next.decode(in, packetSegmentContext);
        }
    }    
}
