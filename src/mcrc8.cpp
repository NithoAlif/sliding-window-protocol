#include "crc8.h"

int main() {
	crc8 cek("0000000000000000000000");
	for (int i = 0; i < nPoly-1; ++i)
	{
		printf("%c", cek.CheckSum[i]);
	}
	printf("\n");
	return 0;
}
