#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "frame.cpp"

using namespace std;

int main() {
    struct sockaddr_in myaddr;
    struct sockaddr_in remaddr;
    socklen_t slen = sizeof(remaddr);
    int s;
    string server = "127.0.0.1";
    
    // Create UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Cannot create socket. \n");
        exit(1);
    }

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);

    // Bind it to local address and pick any port number
    if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("Bind failed");
        return 0;
    }

    // Define remote address whom we want to send messages
    memset((char *) &remaddr, 0, sizeof(remaddr));
    remaddr.sin_family = AF_INET;
    remaddr.sin_port = htons(2000);
    
    // Convert IPV4 numbers and dots notation into binary form
    if (inet_aton(server.c_str(), &remaddr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Everything is configured and ready to send the message
    char str[16] = "haloooo";
    frame f(1, str);

    // Copy serialized data from frame
    char result[DATASIZE + 15];
    for (int i = 0; i < DATASIZE + 15; i++) {
        result[i] = f.getResult()[i];
    }
    
    printf("Sending packet..."); 
    if (sendto(s, result, 32, 0, (struct sockaddr *)&remaddr, slen)==-1) {
        perror("sendto");
        exit(1);
    }
    return 0;
}
