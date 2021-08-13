#include "Wrapper.h"
#include "Packet.h"
#include "Control.h"
#include "PacketSegmentContext.h"
#include "ControlParameters.h"

class ControlWrapper : public Wrapper
{
       
    ControlWrapper();
    ~ControlWrapper(); 
    void beforeEncode(Packet in, Control* control, Data* dataSeg);
    void encode(Packet in,PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas);
                       
 private:      
    static  string MULTIPLECOMMAND ;

};