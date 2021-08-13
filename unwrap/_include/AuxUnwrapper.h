#include "Unwrapper.h"
#include "PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"
#include "BcdUnsigned.h"
#include "FieldTypeParam.h"



class AuxUnwrapper : public Unwrapper
{
   
    
    public:    
    void decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext);
    
    private:

};