#include <iostream>
#include "dcomm.h"
#include "frame.cpp"
#include "crc8.h"

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

	response (char source[5 + CHECKSUMSIZE]) {
		// Set sign
		sign = source[0];
		
		// Set frameNumber
		char tmp[sizeof(int)];
		for (int i = 0; i < sizeof(int); i++) {
			tmp[i] = source[i + 1];
		}
		frameNumber = *(int *)tmp;

		// Set checksum
		string checksum_tmp;
		for (int i = 0; i < CHECKSUMSIZE; i++) {
			checksum_tmp.push_back(source[5 + i]);
		}
		checksum = checksum_tmp;

		// Set result
		for (int i = 0; i < 5 + CHECKSUMSIZE; i++) {
			result[i] = source[i];
		}
	}

	response(frame frame_) {
		frameNumber = frame_.getFrameNumber();
		string checksum_tmp; 
		for (int j = 0; j < DATASIZE + 6; j++) { 
			checksum_tmp.push_back(frame_.getResult()[j]); 
		} 
		crc8 crcFrame(checksum_tmp); 
		string cs = crcFrame.hexCS;

		if (cs[0] == frame_.getChecksum()[0] && cs[1] == frame_.getChecksum()[1]){
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
		crc8 crcResponse(tmp);
		checksum = crcResponse.hexCS;


//		checksum = crc32(tmp);
		
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
	void setChecksum(string s) {
		crc8 crc(s);
		checksum = crc.hexCS;
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