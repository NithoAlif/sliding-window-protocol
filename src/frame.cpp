#include <iostream>
using namespace std;

class frame{
public:
	frame();
	~frame();

	void setSOH(char c){
		soh = c;
	}
	void setFrameNumber(int i){
		frameNumber = i;
	}
	void setSTX(char c){
		stx = c;
	}
	void setData(char c){
		data = c;
	}
	void setETX(char c){
		etx = c;
	}
	void setChecksum(string s){
		checksum = s;
	}

	char getSOH(){
		return soh;
	}
	int getFrameNumber(){
		return i;
	}
	char getSTX(){
		return stx;
	}
	char getData(){
		return data;
	}
	char getETX(){
		return etx;
	}
	char getChecksum(){
		return checksum;
	}

private:
	char soh;
	int frameNumber;
	char stx;
	char data;
	char etx;
	string checksum;

};