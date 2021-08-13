#include "WrapperChain.cpp"

 WrapperChain::WrapperChain()
 {

 }

 WrapperChain::~WrapperChain()
 {
    if(wrappers.size()>0)
    {
        Wrapper* pre = wrappers.pop_front();
        delete pre;
    }    
    wrappers.clear();

 }


void WrapperChain::add(Wrapper* wrapper)
{
    wrappers.push_back(wrapper);
}

void WrapperChain::encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas)
{
    if(wrappers.size()>0)
    {
        Wrapper* pre = wrappers.pop_front();
        pre->encode(in,packetSegmentContext,controlparas);
    }    
}
