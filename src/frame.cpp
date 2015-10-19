#include <iostream>
#include "dcomm.h"
using namespace std;

class frame{
public:
	frame() : soh(SOH), stx(STX), etx(ETX){
		frameNumber = 0;
		data = "";
		setChecksum();
	}
	
	frame(int i, string s) : soh(SOH), stx(STX), etx(ETX){
		frameNumber = i;
		data = s;
		setChecksum();
	}
	
	frame (char a[32]){
		soh = a[1];
		
		char tmp[4];
		for (int i = 0; i < 4; ++i){
			tmp[i] = a[2+i];
		}
		frameNumber = atoi(tmp);
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
	void setData(string s){
		data = s;
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
	string getData(){
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
	string data;
	char etx;
	string checksum;
	char result[32];
};