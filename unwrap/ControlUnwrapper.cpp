#include "ControlUnwrapper.h"

int ControlUnwrapper::CONTROL_LENGTH = 10;
int ControlUnwrapper::HEAD_LENGTH = 6;

ControlUnwrapper::ControlUnwrapper()
{

}

void ControlUnwrapper::decode(unsigned char in[], int nLen, PacketSegmentContext packetSegmentContext)
{
       
        Control control =(Control)packetSegmentContext.getSegment(SegmentEnum::CONTROL);    
      
        //控制域功能码,帧计数有效位,fcb帧计数位,启动标志位,传输方向位 解码
        unsigned char b1 =in[HEAD_LENGTH];
        control.setFunctionCode(b1&0x0f);
        control.setFcv((b1>>4) & 0x01);
        control.setFcbOrAcd((b1>>5) & 0x01);
        control.setPrm((b1>>6) & 0x01);
        control.setDir((b1>>7) & 0x01);

        //地址解码
        unsigned char addr1[2];
        unsigned char addr2[2];
        
        addr1[0] = in[HEAD_LENGTH +1];
        addr1[1] = in[HEAD_LENGTH +2];
        addr2[0] = in[HEAD_LENGTH +3];
        addr2[1] = in[HEAD_LENGTH +4];

        int admin_code = ((addr1[1] & 0xF0) * 16 + (addr1[1] & 0x0F)) << 8 + (addr1[0] & 0xF0) * 16 + (addr1[0] & 0x0F);    
        int concentrateNo = ((addr2[1] & 0xF0) * 16 + (addr2[1] & 0x0F)) << 8 + (addr2[0] & 0xF0) * 16 + (addr2[0] & 0x0F);    

        control.setAdmin_code(admin_code);        
        control.setConcentrateNo(concentrateNo);

        //主站地址，afn解码
        unsigned char b2= in[HEAD_LENGTH + 5];        
        control.setIsGroup(b2 & 0x01);
        control.setMsa((b2>>1) & 0x7f);
        control.setAfn(in[HEAD_LENGTH + 6]);

        //帧序列，是否需确认，是否报文第一帧，是否报文最后一帧，附加信息是否有时间标签 解码
        unsigned char b3 = in[HEAD_LENGTH + 7];
        control.setSeq(b3 & 0x0f);
        control.setIsNeedConfirm((b3>>4) & 0x01);
        control.setFin((b3>>5) & 0x01);
        control.setFir((b3>>6) & 0x01);
        control.setTpV((b3>>7) & 0x01);

        control.setDecodeCompleted();
        
        if(next!=null){
            next.decode(in,packetSegmentContext);
        }
    
}
