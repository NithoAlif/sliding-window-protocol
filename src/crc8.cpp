/* 	
	Nama file : crc8.cpp
	Deskripsi : Mengenerate checksum dari frame menggunakan aturan CRC-8
*/

#include "crc8.h"
using namespace std;

crc8::crc8(string chf) {
	Error = false;
	strcpy(Polynomial, "100110001");
	memset(CheckSum,nPoly-1,0);
	memset(Frame,nFrame,0);
	a = chf.length();
	a *= 8;
	SFrame = chf;
	convertFrame();
	crc();
	for (int i = a; i < a+nPoly-1; ++i)
	{
		setElmtFrame(i, getElmtCheckSum(i-a));
	}
	hexingCheckSum();
}

crc8::~crc8() {
	//memset()
}

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
		setElmtCheckSum(j, getElmtFrame(i++));
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
	for (int i = nFrame; i > nFrame-8; --i)
	{
		setElmtFrame(i-1,'0');
	}
}

void crc8::hexingCheckSum() {
	int temp = 0; int nPow = 0; char res;
	for (int i = nPoly-2; i >= 0; --i)
	{
		int power = 1;
		for (int j = 0; j < nPow; ++j)
		{
			power *= 2;
		}
		nPow = (nPow + 1) % 4;
		temp += (CheckSum[i] - '0') * power;
		if (nPow == 0)
		{
			if (temp < 10)
			{
				res = temp + '0';
			}
			else {
				switch (temp) {
					case 10 : res = 'A'; break;
					case 11 : res = 'B'; break;
					case 12 : res = 'C'; break;
					case 13 : res = 'D'; break;
					case 14 : res = 'E'; break;
					case 15 : res = 'F'; break;
					default : break;
				}
			}
			temp = 0;
			hexCS = res + hexCS;
		}
	}
}

void isError() {

}
