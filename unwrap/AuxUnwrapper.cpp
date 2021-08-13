#include "AuxUnwrapper.h"

void AuxUnwrapper::decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext) 
{
        Control control =(Control)packetSegmentContext->getSegment(SegmentEnum::CONTROL);
        Auxiliary auxiliary =(Auxiliary)packetSegmentContext->getSegment(SegmentEnum::AUXILIARY);

        if(control.getFcbOrAcd()==1)
        {
            auxiliary.setEc1(in[nLen-10]);
            auxiliary.setEc2(in[nLen-9]);
        }

        if(control.getTpV()==1)
        {
            auxiliary.setPfc(in[nLen - 8]);
            unsigned char Tp[4];
            Tp[0] = in[nLen - 4];
            Tp[1] = in[nLen - 5];
            Tp[2] = in[nLen - 6];
            Tp[3] = in[nLen - 7];

            auxiliary.setTp(Tp);
            auxiliary.setTimeout(in[nLen - 3]);
        }

        
        auxiliary.setDecodeCompleted();
        
        if(next!=null){
            next.decode(in, nLen, packetSegmentContext);
        }    

}
