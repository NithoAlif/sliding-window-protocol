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
int* status_table;
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

    status_table = new int[frame_vector.size()];
    for (int i = 0; i < frame_vector.size(); i++) {
        status_table[i] = -1;
    }
}

void sendMessage() {
    openFile("/home/husni/Desktop/SlidingWindow/sliding-window-protocol/bin/file.txt");

    int firstWindow = 0;
    int lastWindow = WINDOWSIZE;
    int i = firstWindow;
    while (firstWindow != lastWindow) {
        for(int i = firstWindow; i < lastWindow; i++) {
            if (status_table[i] == -1) {

                printf("Sending frame %d\n", i); 

                cout << frame_vector[i].getFrameNumber() << " " <<frame_vector[i].getChecksum() << endl;
                if (sendto(s, frame_vector[i].getResult(), 32, 0, (struct sockaddr *)&remaddr, slen)==-1) {
                    perror("sendto");
                    exit(1);
                }
                status_table[i] = 0;
            }

            if (status_table[firstWindow] == 1) {
                firstWindow++;
                if (lastWindow == frame_vector.size()) {
                    lastWindow = frame_vector.size();
                } else {
                    lastWindow++;
                }

                // cout << "First Window: " << firstWindow << endl;
                // cout << "Last Window:" << lastWindow << endl;
            }
            usleep(50);
        }
    }
}

void receiveSignal(){
    for(;;){
        char c[10];
        int recvlen = recvfrom(s, c, 10, 0, (struct sockaddr *)&remaddr, &slen);
        if (recvlen > 0) {
            string str(c);
            int frameNumber = stoi(str);
            // ACK diterima
            status_table[frameNumber] = 1;
            cout << "ACK " << frameNumber << endl;
        }
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
    receiveThread.join();

    return 0;
}
