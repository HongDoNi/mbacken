
#include <list>
#include <string>
#include <map>

using namespace std;


enum SegmentEnum {
    AUXILIARY = 0,
    CONTROL,
    DATA,
    HEAD,
    TAIL
};

class Segment 
{
    
    public:

    Segment(SegmentEnum segmentID)
    {
        this->segmentID = segmentID;
        isDecoded = false;
        buffer = NULL;
        nLen = 0;
    }

    virtual ~Segment()
    {
        if(buffer)
            delete []buffer;
    }

    void getBuffer(unsigned char* buff, int* len) 
    {
        *len = nLen;
        memcpy(buff, buffer, nLen);
    }

    void setBuffer(unsigned char* buf, int n) 
    {
        this->nLen = n;
        this->buffer = new unsigned char[n];
        memcpy(buffer, buf, n);
    }

    SegmentEnum getSegmentID()
    {
        return segmentID;
    }

     bool isDecodeCompleted()
    {
        return isDecoded;
    }

    void setDecodeCompleted()
    {
        isDecoded = true; 
    }

    virtual void reset()
    {
        isDecoded = false;
        delete []buffer;
        nLen = 0;
    }

    protected:
    unsigned char* buffer;
    int nLen;
    SegmentEnum segmentID;
    bool isDecoded;

};