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
		usleep(500);
		
		
	}
}

void consumeMessage(){
	for (;;){
		for (int i = 0; i < RXQSIZE; i++) {
			
			if (frame_buffer[i].getFrameNumber() != -1){
				//if (frame_buffer[i].getChecksum() == "lol"){
					cout << "consume nih!" << endl;
					cout << frame_buffer[i].getData() << endl;
			
					char c = NAK;
					//sendto(s, &c, 1, 0, (struct sockaddr *)&remaddr, addrlen);
		
					if (sendto(s, &c, 1, 0, (struct sockaddr *)&remaddr, addrlen)==-1) {
					    perror("sendto");
					    exit(1);
					}
					sleep(3);
					//frame_buffer[i].empty();
					cout << frame_buffer[i].getFrameNumber() << endl;
					
				//} 
			}
		}
	}
}


#define BUFSIZE 32

int main(int argc, char const *argv[]) {

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
	thread receive_t(receiveMessage);
	thread consume_t(consumeMessage);
	consume_t.join();
	receive_t.join();
	// Everything is configured and ready to receive message
	
}
