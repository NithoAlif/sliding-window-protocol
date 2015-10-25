#include <iostream>
#include "dcomm.h"
#include "frame.cpp"

using namespace std;

class response {
public:
	response() {
		sign = NAK;
		frameNumber = -1;
		checksum = "";
	}

	response(char _sign, int _frameNumber, string _checksum) {
		sign = _sign;
		frameNumber = _frameNumber;
		checksum = _checksum;
	}

	response(frame frame_) {
		frameNumber = frame_.getFrameNumber();

		char cc[DATASIZE+7];
		for (int i = 0; i < DATASIZE+7; ++i){
			cc[i] = frame_.getData()[i];
		}
		
		CRC32 crc32;
		string cs = crc32(cc);
		if (cs == frame_.getChecksum()){
			sign = ACK;
		} else{
			sign = NAK;
		}	

		result[0] = sign;
			
		// Serialize frameNumber
		result[1] = frameNumber & 0xFF;
		result[2] = (frameNumber >> 8) & 0xFF;
		result[3] = (frameNumber >> 16) & 0xFF;
		result[4] = (frameNumber >> 24) & 0xFF;

		string tmp;
		for (int i = 0; i < 5; ++i){
			tmp[i] = result[i];
		}
		checksum = crc32(tmp);
		
		// Serialize checksum
		for (int i = 0; i < CHECKSUMSIZE; i++) {
			result[5 + i] = checksum[i];
		}		

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

	string getChecksum() {
		return checksum;
	}
	void setChecksum(string _checksum) {
		CRC32 crc32;
		checksum = crc32(_checksum);
	}
	char* getResult() {
		return result;
	}

private:
	char sign;
	int frameNumber;
	string checksum;
	char result[5 + CHECKSUMSIZE];
};