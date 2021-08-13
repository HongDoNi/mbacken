#include "Unwrapper.h"
#include "PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"


class ControlUnwrapper : public Unwrapper
{  
   
    public:
    ControlUnwrapper();
    void decode(unsigned char in[], int nLen, PacketSegmentContext packetSegmentContext);

    private:
    static  int CONTROL_LENGTH;
    static  int HEAD_LENGTH;

};