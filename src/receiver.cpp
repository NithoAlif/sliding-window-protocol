#include <iostream>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "dcomm.h"
#include "frame.cpp"
#include "crc32.h"
using namespace std;


struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t addrlen = sizeof(remaddr);
int s;
int msgcnt = 0;
char sent_frame[DATASIZE + 15];
frame frame_buffer[RXQSIZE];


void receiveMessage(){
	printf("waiting on port 9999\n");
	for (;;) {
		int recvlen = recvfrom(s, sent_frame, sizeof(sent_frame), 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) {
			frame f(sent_frame);
			frame_buffer[f.getFrameNumber() % WINDOWSIZE] = f;
		}
		
	}
}

void consumeMessage(){
	for (;;){
		for (int i = 0; i < RXQSIZE; i++) {
			if (frame_buffer[i].getFrameNumber() != -1) { // jika frame buffer tidak kosong
				
				CRC32 crc32;
				string checksum_tmp;
				for (int j = 0; j < DATASIZE + 6; j++) {
					checksum_tmp.push_back(frame_buffer[i].getResult()[j]);
				}
				
				cout << "received checksum: " << crc32(checksum_tmp) << endl;
				cout << "real checksum: " << frame_buffer[i].getChecksum() << endl;

				if (1) { // jika checksum sama
					int frameNumber = frame_buffer[i].getFrameNumber();
					char buf[10];
					sprintf(buf, "%d", frameNumber);
					if (sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, addrlen)==-1) {
					    perror("sendto");
					    exit(1);
					}

				} else { // checksum tidak sama
					// kirim NAK
				}

				// Hapus frame yang sudah diproses
				usleep(50);
				frame_buffer[i].empty();
			}
		}
	}
}

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		cout << "Usage: receiver <port>" << endl;
		return 0;
	}

	// Create UDP socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	// Bind the socket to any valid IP address and a specific port
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(atoi(argv[1]));

	if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	// Everything is configured and ready to receive message
	thread receive_t(receiveMessage);
	thread consume_t(consumeMessage);
	consume_t.join();
	receive_t.join();

	return 0;
}
