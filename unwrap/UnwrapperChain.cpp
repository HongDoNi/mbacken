#include "UnwrapperChain.h"

UnwrapperChain::UnwrapperChain()
{

}

UnwrapperChain::~UnwrapperChain()
{

    clear();
}

void UnwrapperChain::add(Unwrapper* unwrapper)
{
    //if(unwrappers.size()>0)
    //{
    //        Unwrapper* pre = unwrappers.get(unwrappers.size()-1);
     //       pre->setNext(unwrapper);
    //}

    unwrappers.push_back(unwrapper);
}

void UnwrapperChain::decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext)
{
    if(unwrappers.size()>0){
       Unwrapper* unwrapper = unwrappers.pop_front();
       unwrapper->decode(in, nLen, packetSegmentContext);
    }    
}

void UnwrapperChain::clear()
{
     while(unwrappers.size()>0)
     {
          Unwrapper* unwrapper = unwrappers.pop_front();
          delete unwrapper;
     }
}