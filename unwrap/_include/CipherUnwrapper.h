#include "Unwrapper.h"
#include "PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"



class CipherUnwrapper : public Unwrapper
{
   
    public:
    void decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext);

    private：
    static int CONTROL_LENGTH;
    static int HEAD_LENGTH;
};
    