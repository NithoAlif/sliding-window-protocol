#include <cstring>
#include "dcomm.h"
using namespace std;

class frame{
public:
	
	frame() : soh(SOH), stx(STX), etx(ETX) {
		frameNumber = 0;
		string s = "";
		strcpy(data, s.c_str());
		// setChecksum();
	}

	frame(int i, string s) : soh(SOH), stx(STX), etx(ETX) {
		frameNumber = i;
		strcpy(data, s.c_str());
		// setChecksum();
	}

	frame (char a[DATASIZE+15]){
		
	}


	~frame(){

	}

	void setSOH(char c){
		soh = c;
	}
	void setFrameNumber(int i){
		frameNumber = i;
	}
	void setSTX(char c){
		stx = c;
	}
	void setData(string c){
		
	}
	void setETX(char c){
		etx = c;
	}
	void setChecksum(){
		//checksum = s;
	}

	char getSOH(){
		return soh;
	}
	int getFrameNumber(){
		return frameNumber;
	}
	char getSTX(){
		return stx;
	}
	char* getData(){
		return data;
	}
	char getETX(){
		return etx;
	}
	string getChecksum(){
		return checksum;
	}

private:
	char soh;
	int frameNumber;
	char stx;
	char data[DATASIZE];
	char etx;
	string checksum;
	char result[32];
};