#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cstring>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "frame.cpp"

using namespace std;

struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t slen = sizeof(remaddr);
vector<frame> frame_vector;
string server = "127.0.0.1";
int s;

void openFile(string filename) {
    ifstream fin;
    fin.open(filename.c_str(), ios::in);

    int charCounts = 0;
    int frameNumber = 0;
    
    char current;
    char tmp_char[DATASIZE];
    memset(tmp_char, 0, sizeof(tmp_char));

    while (fin.get(current)) {
        cout << current;
        charCounts++;
        tmp_char[(charCounts-1) % (DATASIZE - 1)] = current; 
        
        if (charCounts % (DATASIZE - 1) == 0) {
            frame f(frameNumber, tmp_char);
            frame_vector.push_back(f);
            frameNumber++;

            memset(tmp_char, 0, sizeof(tmp_char));
        }
    }
    
    if (charCounts % (DATASIZE - 1) != 0) {
        frame f(frameNumber, tmp_char);
        frame_vector.push_back(f);
    }
}

void sendMessage() {
    openFile("/home/husni/Desktop/SlidingWindow/sliding-window-protocol/bin/file.txt");

    // Send message 
    for (int i = 0; i < frame_vector.size(); i++) {
        char result[DATASIZE + 15];
        for (int j = 0; j < DATASIZE + 15; j++) {
            result[j] = frame_vector[i].getResult()[j];
        }

        printf("Sending frame %d\n", i); 
        if (sendto(s, result, 32, 0, (struct sockaddr *)&remaddr, slen)==-1) {
            perror("sendto");
            exit(1);
        }
    }
}

void receiveSignal(){
    for(;;){
        char c[10];
        int recvlen = recvfrom(s, c, 10, 0, (struct sockaddr *)&remaddr, &slen);
        cout << c << endl;
    }
}

int main() {
    
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
    thread sendMessageThread(sendMessage);
    thread receiveThread(receiveSignal);
    sendMessageThread.join();
    receiveThread.join();

    return 0;
}
