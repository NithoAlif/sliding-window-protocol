#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "dcomm.h"
#include "frame.cpp"
using namespace std;

#define BUFSIZE 32

int main() {
    struct sockaddr_in myaddr;
    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    int s;
    int msgcnt = 0;
    char buf[DATASIZE + 15];


    // Create UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        return 0;
    }

    // Bind the socket to any valid IP address and a specific port
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(2000);

    if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }

    // Everything is configured and ready to receive message
    for (;;) {
        printf("waiting on port 2000\n");
        int recvlen = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            printf("received message: (%d bytes)\n", recvlen);
        }

        frame f(buf);
        printf("Data: %s\n", f.getData());
        printf("Frame number: %d\n", f.getFrameNumber());
    }
}
