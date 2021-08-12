#include "Segment.h"


class Auxiliary : public Segment
{

public:

    Auxiliary(SegmentEnum segmentID);

    ~Auxiliary();

    int getTimeout() {
        return timeout;
    }

    void setTimeout(int timeout) {
        this->timeout = timeout;
    }

     string getPassword() {
        return password;
    }

    void setPassword(string password) {
        this->password = password;
    }

    int getEc1() {
        return ec1;
    }

    void setEc1(int ec1) {
        this->ec1 = ec1;
    }

    int getEc2() {
        return ec2;
    }

    void setEc2(int ec2) {
        this->ec2 = ec2;
    }

    int getPfc() {
        return pfc;
    }

    unsigned char* getTp()
    {
        return Tp;
    }
    
    void setTp(unsigned char Tp[], int nLen);

    void reset();

    private:
    string password;
    int ec1;
    int ec2;
    int pfc;
    unsigned char Tp[4];
    unsigned char PW[16];
    int timeout;
};
