#include "ProtocalImpl.h"

int ProtocalImpl::SEQ_NOT_MATCH   =2;
int ProtocalImpl::TIMEOUT_REACH   =4;
int ProtocalImpl::HAVE_NEXT_FRAME =8;

ProtocalImpl::ProtocalImpl()
{
    decoder = new Decoder;
    encoder = new Encoder;
    lastReturn = 1;    
    pfc=0;
}


ProtocalImpl::~ProtocalImpl()
{

    if(decoder)
        delete decoder;
    if(encoder)
        delete encoder;

}

//开始解码
        @Transit(from= ProtocalState.IDLE, to= ProtocalState.IDLE,
                on=Action.DECODE),
        //开始编码
        @Transit(from= ProtocalState.IDLE, to= ProtocalState.WAIT_RESPONSE,
                on=Action.ENCODE),
        //主动上传，忽略
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.WAIT_RESPONSE,
                on=Action.DECODE,when=InitiativeUploadCondition.class),
        //收到回复，没有多帧
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.IDLE,
                on=Action.DECODE,when=SingleEncodeCondition.class),
        //等待多帧解码
        @Transit(from= ProtocalState.WAIT_RESPONSE, to= ProtocalState.WAIT_NEXT_DECODE,
                on=Action.DECODE,when=MultipleDecodeCondition.class),
        //等待多帧解码
        @Transit(from= ProtocalState.WAIT_NEXT_DECODE, to= ProtocalState.WAIT_NEXT_DECODE,
                on=Action.DECODE,when=MultipleDecodeCondition.class),
        //完成最后一帧解码
        @Transit(from= ProtocalState.WAIT_NEXT_DECODE, to= ProtocalState.IDLE,
                on=Action.DECODE,when=SingleEncodeCondition.class),
  
int ProtocalImpl::encode(Packet in,BinPacket out)
{
        //流程控制
        if(codeConfig.isProcessControl()) {
            if (protocalStatus == 0) {
                pfc++;
                lastReturn = encoder->encode(in, out, pfc);
                protocalStatus = 1;
                if (in.getTerminalAddress() == "") {
                    lastEncodedTerminalAddress = codeConfig.getTerminalAddress();
                } else {
                    lastEncodedTerminalAddress = in.getTerminalAddress();
                }
            }
            return lastReturn;
        }else {
            pfc++;
            return encoder->encode(in, out, pfc);
        }

}


int ProtocalImpl::decode(unsigned char in[], int nLen, Packet out)
{
    
    int result = decoder->decode(in, nLen, out, pfc);
    
    //帧序号不匹配。pfc重置
    if((SEQ_NOT_MATCH&result)==SEQ_NOT_MATCH || (TIMEOUT_REACH&result)==TIMEOUT_REACH)
    {
        pfc=0;
    }

    //非同一终端，无返回数据
    if(lastEncodedTerminalAddress != "" && lastEncodedTerminalAddress != out.getTerminalAddress())
    {
            out.setCommand("");            
    }

    //收到回复帧且是最后一帧，编码打开
    if((HAVE_NEXT_FRAME&result)!=HAVE_NEXT_FRAME)
    {
        protocalStatus = 0;
    }
    else
    {
        //多帧,帧序号递增
        pfc++;
        
    }

    return result;
}

