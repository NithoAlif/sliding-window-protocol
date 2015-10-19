#include <iostream>
#include <cstdio>
#include <cstring>

#include "dcomm.h"
using namespace std;

class frame{
public:
	
	frame() : soh(SOH), stx(STX), etx(ETX) {
		frameNumber = -1;
		data[0] = '\0';
		// setChecksum();
	}

	frame(int i, char s[DATASIZE]) : soh(SOH), stx(STX), etx(ETX) {
		frameNumber = i;
		strcpy(data, s);
		// setChecksum();

		// Serialize frame to array of char
		result[0] = soh;
		result[5] = stx;
		result[6 + DATASIZE] = etx;

		result[1] = i & 0xFF;
		result[2] = (i >> 8) & 0xFF;
		result[3] = (i >> 16) & 0xFF;
		result[4] = (i >> 24) & 0xFF;

		for (int i = 0; i < 8; i++) {
			result[7 + DATASIZE + i] = checksum[i];
		}

		for (int i = 0; i < DATASIZE; i++) {
			result[6 + i] = data[i];
		}
	}

	frame (char source[DATASIZE+15]) {
		soh = source[0];
		stx = source[5];
		etx = source[6 + DATASIZE];

		char tmp[sizeof(int)];
		for (int i = 0; i < sizeof(int); i++) {
			tmp[i] = source[i+1];
		}

		frameNumber = *(int *)tmp;

		for (int i = 0; i < DATASIZE; i++) {
			data[i] = source[6 + i];
		}

		strcpy(result, source);

	}

	~frame(){
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
		checksum = s;
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
	char result[DATASIZE + 15];
};