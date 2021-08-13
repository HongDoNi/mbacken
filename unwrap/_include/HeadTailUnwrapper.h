#include "Unwrapper.h"
#include "PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"

class HeadTailUnwrapper : public Unwrapper
{
   
public:
    void decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext);
                       
    private:
    static int HEAD_LENGTH;
    static int TAIL_LENGTH;

};
