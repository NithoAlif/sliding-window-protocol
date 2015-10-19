#include <iostream>
#include "dcomm.h"

class response {
public:
    response() {
        sign = NAK;        
        frameNumber = -1;
        checksum = "";
    }

    response(char _sign, int _frameNumber, std::string _checksum) {
        sign = _sign;
        frameNumber = _frameNumber;
        checksum = _checksum;
    }

    ~response() {

    }

    char getSign() {
        return sign;
    }

    void setSign(char _sign) {
        sign = _sign;
    }

    int getFrameNumber() {
        return frameNumber;
    }

    void setFrameNumber(int _frameNumber) {
        frameNumber = _frameNumber;
    }

    std::string getChecksum() {
        return checksum;
    }

    void setChecksum(std::string _checksum) {
        checksum = _checksum;
    }

private:
    char sign;
    int frameNumber;
    std::string checksum;
};