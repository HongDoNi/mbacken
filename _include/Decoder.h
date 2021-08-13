#include "unwrap/UnwrapperChain.h"
#include "packetSegment/PacketSegmentContext.h"
#include "ProtocalTemplate.h"
#include "CodeConfig.h"
#include "ConfigParse/Constants.h"

class Decoder:public Constants
{

    //解码链
    
public:
    
    Decoder();
    ~Decoder();
    int decode( unsigned char in[], int nLen, Packet out, int pfc); 

private:
    UnwrapperChain unwrapperChain;
   
   
    static int SEQ_NOT_MATCH;
    static int TIMEOUT_REACH;
    static int HAVE_NEXT_FRAME;
    static int NEED_CONFIRM=;
    static int INITIATIVE_UPLOAD;
    static int HAVE_EVENT;
    static int BROADCAST;
};
