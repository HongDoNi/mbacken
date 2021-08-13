#include "Wrapper.h"
#include "Constants.h"
//#include "Da.h"
//#include "Dt.h"
#include "../package/PacketSegmentContext.h"
#include "ControlParameters.h"
#include <map>

using namespace std;

class DataWrapper : public Wrapper
{
public:
    DataWrapper();
    void encode(Packet in, PacketSegmentContext* packetSegmentContext, ControlParameters* controlparas); 
private:
    static string MULTIPLECOMMAND;
//    Da da;
//    Dt dt; 
    
};
