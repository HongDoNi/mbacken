#include <map>
#include <string>

#include "Unwrapper.h"
#include "../fieldType/Da.h"
#include "../fieldType/Dt.h"
//#include "Formatter.h"
//#include "../ConfigParse/FieldTypeParam.h"
#include "../fieldType/FieldTypeContext.h"



//#include "FieldGroup.h"
#include "Unwrapper.h"
#include "../packageSegment/PacketSegmentContext.h"
#include "../ProtocolTemplate.h"
#include "../CodeConfig.h"

using namespace std;

class DataUnwrapper : public Unwrapper 
{
    
public:
    DataUnwrapper();
    void init();
    void decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext);
    
    unsigned char* DAencode(string value);
    int DAdecode(unsigned char byteBuffer1, unsigned char byteBuffer2);

    unsigned char* DTencode(string value);
    int DTdecode(unsigned char byteBuffer1, unsigned char byteBuffer2);

    private:
    void decodeData1(unsigned char in[], int nLen, Data* dataSeg,Control* control);
    void decodeData2(unsigned char in[], int nLen, Data* dataSeg,Control* control);   
    void decodeEvent(unsigned char in[], int nLen, Data* dataSeg,Control* control);
    void decodeSetting(unsigned char in[], int nLen, Data* dataSeg,Control* control);

private:
    Da da;
    Dt dt;
     
    static int AFN_EVENT;
    static int AFN_DATA1;
    static int AFN_DATA2;
    static int AFN_SETTING;
};
