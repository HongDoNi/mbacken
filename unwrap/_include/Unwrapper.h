
#include "../packetSegment/PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"

using namespace std;

class Unwrapper 
{

    public:

    virtual void decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext) = 0;

    void setNext(Unwrapper* unwrapper){
        next=unwrapper;
    }

    protected:
    Unwrapper* next;

};
