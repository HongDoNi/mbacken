#include <sstream>
#include "Decoder.h"

int Decoder:: SEQ_NOT_MATCH    = 2;
int Decoder:: TIMEOUT_REACH    = 4;
int Decoder:: HAVE_NEXT_FRAME  = 8;
int Decoder:: NEED_CONFIRM     = 16;
int Decoder:: INITIATIVE_UPLOAD= 32;
int Decoder:: HAVE_EVENT       = 64;
int Decoder:: BROADCAST        = 128;


Decoder::Decoder()
{
    unwrapperChain.add(new HeadTailUnwrapper());
    unwrapperChain.add(new ControlUnwrapper());
    unwrapperChain.add(new CipherUnwrapper());
    unwrapperChain.add(new DataUnwrapper());
    unwrapperChain.add(new AuxUnwrapper());
}

int Decoder::decode(unsigned char in[], int nLen, Packet out, int pfc)
{
        
        PacketSegmentContext packetSegmentContext;
        unwrapperChain.decode(in, nLen, packetSegmentContext);
        
        int result=1;

        Control* control     =(Control*)packetSegmentContext.getSegment(SegmentEnum::CONTROL);
        Auxiliary* auxiliary =(Auxiliary*)packetSegmentContext.getSegment(SegmentEnum::AUXILIARY);
        Data* data           =(Data*)packetSegmentContext.getSegment(SegmentEnum::DATA);

        //设置解码结果
        out.setCommand(data.getCommand());
        out.setLine(data.getPn());
        out.setData(data);

        std::ostringstream buffer;
        buffer << control->getAddress1() << "| " << control->getAddress2()).toUpperCase();

        out.setTerminalAddress(buffer.str()) ;// String.format("%s%s", control.getAddress1(), control.getAddress2()).toUpperCase());

        //设置返回值
        if(control->getPrm()==1 && control->getDir()==1)
        {
            result+=INITIATIVE_UPLOAD;
            if(control->getIsNeedConfirm()==1){
                result+=NEED_CONFIRM;
            }
        }else{
            if(control->getSeq()!=(pfc&0x0f)){
                result+=SEQ_NOT_MATCH;
            }
        }


        if(control->getTpV()==1){
            //if(System.currentTimeMillis()-auxiliary.getSendTime()>auxiliary.getTimeout()*60000){
                result+=TIMEOUT_REACH;
            //}
        }

        if(control->getFir()==0){
            result+=HAVE_NEXT_FRAME;
        }


        if(control->getFcbOrAcd()==1){
            result+=HAVE_EVENT;
        }

        if(control->getIsGroup()==1){
            result+=BROADCAST;
        }
        
        //清空编码结果
        packetSegmentContext.reset();
        return result;
    
}
