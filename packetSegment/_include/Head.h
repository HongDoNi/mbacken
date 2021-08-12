#include "Segment.h"


class Head : public Segment
{
    

    public:
    Head(SegmentEnum segmentID);

    int getLength() {
        return length;
    }

    void setLength(int length) {
        this->length = length;
    }

    int getVersion() {
        return version;
    }

    void setVersion(int version) {
        this->version = version;
    }

    unsigned char getheadTag()
    {
        return headTag;
    }

    void setheadTag(unsigned char headTag)
    {
        this->headTag = headTag;
    }

    void reset();

    private:
    int length;
    int version;
    unsigned char headTag;

    
};
