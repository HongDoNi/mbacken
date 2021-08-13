#include "Unwrapper.h"
#include <list>



class UnwrapperChain
{
   
    public:
    UnwrapperChain();
    ~UnwrapperChain();
    void add(Unwrapper* unwrapper);
    void decode(unsigned char in[], int nLen,  PacketSegmentContext* packetSegmentContext);
    void clear();
    private:
    std::list<*Unwrapper> unwrappers;
    
};
