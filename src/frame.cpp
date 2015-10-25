#include <iostream>
#include <cstdio>
#include <cstring>

#include "dcomm.h"
#include "crc8.h"

using namespace std;

class frame {
public:
	
	frame() : soh(SOH), stx(STX), etx(ETX) {
		frameNumber = -1;
		data[0] = '\0';
		//setChecksum();
	}

	frame(int i, char s[DATASIZE]) : soh(SOH), stx(STX), etx(ETX) {
		// Set frameNumber
		frameNumber = i;

		// Set data
		strcpy(data, s);

		// Serialize SOH, STX, and ETX
		result[0] = soh;
		result[5] = stx;
		result[6 + DATASIZE] = etx;

		// Serialize frameNumber
		result[1] = i & 0xFF;
		result[2] = (i >> 8) & 0xFF;
		result[3] = (i >> 16) & 0xFF;
		result[4] = (i >> 24) & 0xFF;

		// Serialize data
		for (int i = 0; i < DATASIZE; i++) {
			result[6 + i] = data[i];
		}

		// SetChecksum from SOH to ETX
		string tmp;
		for (int i = 0; i < DATASIZE + 6; i++) {
			tmp.push_back(result[i]);
		}
		setChecksum(tmp);

		// Serialize checksum
		for (int i = 0; i < CHECKSUMSIZE; i++) {
			result[7 + DATASIZE + i] = checksum[i];
		}

	}

	frame (char source[7 + DATASIZE + CHECKSUMSIZE]) {
		// Set SOH, STX, and EXT
		soh = source[0];
		stx = source[5];
		etx = source[6 + DATASIZE];

		// Set frameNumber
		char tmp[sizeof(int)];
		for (int i = 0; i < sizeof(int); i++) {
			tmp[i] = source[i + 1];
		}
		frameNumber = *(int *)tmp;

		// Set data
		for (int i = 0; i < DATASIZE; i++) {
			data[i] = source[6 + i];
		}

		// Set checksum
		string checksum_tmp;
		for (int i = 0; i < CHECKSUMSIZE; i++) {
			checksum_tmp.push_back(source[7 + DATASIZE + i]);
		}
		checksum = checksum_tmp;

		// Set result
		for (int i = 0; i < 7 + DATASIZE + CHECKSUMSIZE; i++) {
			result[i] = source[i];
		}
	}

	~frame(){
	}

	void empty() {
		frameNumber = -1;
	}
	

	void setSOH(char c) {
		soh = c;
	}
	void setFrameNumber(int i) {
		frameNumber = i;
	}
	void setSTX(char c)  {
		stx = c;
	}
	void setData(string c) {
		
	}
	void setETX(char c) {
		etx = c;
	}
	void setChecksum(string s) {
		crc8 crc(s);
		checksum = crc.hexCS;
	}

	char getSOH() {
		return soh;
	}
	int getFrameNumber() {
		return frameNumber;
	}
	char getSTX() {
		return stx;
	}
	char* getData() {
		return data;
	}
	char getETX() {
		return etx;
	}
	string getChecksum() {
		return checksum;
	}

	char* getResult() {
		return result;
	}

private:
	char soh;
	int frameNumber;
	char stx;
	char data[DATASIZE];
	char etx;
	string checksum;
	char result[7 + DATASIZE + CHECKSUMSIZE];
};