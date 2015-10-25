/* 	
	Nama file : crc8.cpp
	Deskripsi : Mengenerate checksum dari frame menggunakan aturan CRC-8
*/

#include "crc8.h"
using namespace std;

crc8::crc8(string chf) {
	strcpy(Polynomial, "11010101");
	a = chf.length();
	a *= 8;
	SFrame = chf;
	convertFrame();
	crc();
	for (int i = a; i < a+nPoly-1; ++i)
	{
		setElmtFrame(i, getElmtCheckSum(i-a));
	}
}

crc8::~crc8() {}

void crc8::axor() {
	for (int i = 1; i < nPoly; ++i)
	{
		if (getElmtCheckSum(i) == getElmtPolynomial(i))
		{
			setElmtCheckSum(i,'0');
		}
		else {
			setElmtCheckSum(i,'1');
		}
	}
}

void crc8::crc() {
	int i, j;
	for (i = 0; i < nPoly; ++i)
	{
		setElmtCheckSum(i,(getElmtFrame(i)));
	}
	do {
		if (getElmtCheckSum(0) == '1')
		{
			axor();
		}
		for (j = 0; j < nPoly-1; ++j)
		{
			setElmtCheckSum(j, getElmtCheckSum(j+1));
		}
		setElmtCheckSum(j, getElmtFrame(i));
		i++;
	} while (i <= a+nPoly-1);
}

char crc8::getElmtFrame(int idx){
	return Frame[idx];
}

char crc8::getElmtCheckSum(int idx){
	return CheckSum[idx];
}

char crc8::getElmtPolynomial(int idx){
	return Polynomial[idx];
}

void crc8::setElmtFrame(int idx, char ch) {
	Frame[idx] = ch;
}

void crc8::setElmtCheckSum(int idx, char ch) {
	CheckSum[idx] = ch;
}

void crc8::convertFrame() {
	for (int i = 0; i < SFrame.length(); ++i)
	{
		int ctemp = (int) SFrame[i];
		int count = 7;
		while(count >= 0) {
			if (ctemp % 2 == 0)
			{
				setElmtFrame(count+i*8, '0');
			}
			else {
				setElmtFrame(count+i*8, '1');
			}
			ctemp = ctemp / 2;
			count--;
		}
	}
}

void isError() {
	
}
