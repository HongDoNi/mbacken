#include "Segment.h"

class Tail : public Segment
{
    
    public:
    Tail(SegmentEnum segmentID);
    unsigned char getCheckSum() {
        return checkSum;
    }

    void setCheckSum(unsigned char checkSum) {
        this->checkSum = checkSum;
    }

    void reset();

    private:
    unsigned char checkSum;

};
