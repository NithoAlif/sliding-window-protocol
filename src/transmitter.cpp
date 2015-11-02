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

//#include "frame.cpp"
#include "response.cpp"

using namespace std;

struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t slen = sizeof(remaddr);
vector<frame> frame_vector;
int* status_table;
int s;
bool finish = false;

/*
 * Membaca file eksternal kemudian mengubahnya dalam bentuk frame yang siap dikirim
 */
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

    // Create EOF
    char data_eof[DATASIZE];
    memset(data_eof, 0, sizeof(data_eof));
    data_eof[0] = Endfile;
    frame frame_eof(frame_vector.size(), data_eof);
    frame_vector.push_back(frame_eof);

    // Initialize status table to -1
    status_table = new int[frame_vector.size()];
    for (int i = 0; i < frame_vector.size(); i++) {
        status_table[i] = -1;
    }
}

/*
 * Mengirimkan frame ke receiver menggunakan sliding window protocol
 */
void sendMessage(string filename) {
    openFile(filename);

    int firstWindow = 0;
    int lastWindow = WINDOWSIZE;
    int i = firstWindow;
    while (firstWindow != lastWindow) {
        for (int i = firstWindow; i < lastWindow; i++) {
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
            }

            usleep(50);
        }
    }
}


/*
 * Mengubah status table dari frame yang sudah dikirim namun belum mendapatkan ACK
 * Keterangan: status_table bisa bernilai 1, 0, -1
 * 1  : frame sudah mendapatkan ack
 * 0  : frame sudah dikirim namun belum mendapatkan ack
 * -1 : frame belum dikirim atau jika frame mendapatkan nak 
 */
void timeOut() {
    while(!finish) {
        sleep(TIMEOUT);
        for (int i = 0; i < frame_vector.size(); i++) {
            if (status_table[i] == 0) {
                status_table[i] = -1;
            }
        }
    }
}

/*
 * Menerima sinyal yang dikirim oleh receiver.
 * Jika sinyal ACK, ubah status table frame number dari respon tersebut ke 1
 * Jika sinyal NAK, ubah status table frame number dari respon tersebut ke -1 
 */
void receiveSignal() {
    while(!finish){
        char sent_response[5 + CHECKSUMSIZE];
        if (!finish) {
            int recvlen = recvfrom(s, sent_response, sizeof(sent_response), 0, (struct sockaddr *)&remaddr, &slen);
            if (recvlen > 0) {
                 response r(sent_response);
                 if (r.getResult()[0] == ACK) {
                     status_table[r.getFrameNumber()] = 1;
                     cout << "ACK" << endl;
                 } else if (r.getResult()[0] == NAK) {
                     status_table[r.getFrameNumber()] = -1;
                     cout << "NAK" << endl;
                 } else {
                    cout << "LOL" << endl;
                 }
            }
        }
    }
}

int main(int argc, char const *argv[]) {

    if (argc < 4) {
        cout << "Usage: ./transmitter <host> <port> <filename>" << endl;
        return 0;
    }

    string server(argv[1]);
    int port = atoi(argv[2]);
    string filename(argv[3]);

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
    remaddr.sin_port = htons(port);
    
    // Convert IPV4 numbers and dots notation into binary form
    if (inet_aton(server.c_str(), &remaddr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Everything is configured and ready to send the message
    thread sendMessageThread(sendMessage, string(filename));
    thread receiveThread(receiveSignal);
    thread timeOutThread(timeOut);
    sendMessageThread.join();
    receiveThread.join();
    timeOutThread.join();

    return 0;
}
