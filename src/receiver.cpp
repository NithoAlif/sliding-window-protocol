/* File		 : receiver.cpp */
/* Deskripsi : Deklarasi objek receiver */
#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "frame.cpp"
using namespace std;

#define RXQSIZE 8 // Define receive buffer size


class buffer{
public:
	/* CREATE NEW BUFFER */
	buffer() : maxsize(RXQSIZE){
		count = 0;
		data = new frame [maxsize];
		for (int i = 0; i < maxsize; ++i){
			//data[i] = 0;
		}
	}

	/* DELETE BUFFER REGION */
	~buffer(){
		delete [] data;
	}

	/* ADD LAST ELEMENT IN BUFFER */
	/*void add(Byte c){
		if (!isFull()){
			data[count] = c;
			count++;
		}
	}*/

	/* CONSUME FIRST ELEMENT IN BUFFER */
	/*void consume(Byte *c){
		if (!isEmpty()){
			*c = data[0];
			count--;
			for (int i = 0; i < count; ++i){
				data[i] = data[i+1];
			}
			data[count] = 0;
		}
	}*/

	/* GETTER CURRENT BUFFER SIZE */
	int getCount(){
		return count;
	}

	/* GETTER FRAME-i */
	frame getFrame(int i){
		return data[i];
	}

	/* GET BUFFER FULL STATUS */
	bool isFull(){
		return (count == maxsize);
	}

	/* GET BUFFER EMPTY STATUS */
	bool isEmpty(){
		return (count == 0);
	}

private:
	int count; // Current buffer size
	int maxsize; // Maximum buffer size
	frame *data; // Buffer memory region
};





class receiver{
public:
	/* CREATE NEW RECEIVER OBJECT */
	receiver(const char* arg) : port(arg){
		createSocket(); // Create new socket

		bindSocket(); // Bind socket to IP/Port

//		sent_xonxoff[0] = XON; // XON/XOFF state initialization

		doReceive(); // Do the receiving

		closeSocket(); // Close socket
	}

	/* CLOSE RECEIVER SOCKET */
	~receiver(){
		closeSocket();
	}

	/* CREATE SOCKET */
	void createSocket(){
		if ((socket_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			throw "Error opening socket!";
		}
	}

	/* BIND SOCKET */
	void bindSocket(){
		bzero(&receiver_endpoint, sizeof(receiver_endpoint));
		receiver_endpoint.sin_family = AF_INET;
		receiver_endpoint.sin_addr.s_addr = htonl(INADDR_ANY);
		receiver_endpoint.sin_port = htons(atoi(port));
		if (bind(socket_, (struct sockaddr *)&receiver_endpoint, sizeof(receiver_endpoint)) < 0){
			throw "Error binding. Try different port!";
		} else{
			inet_ntop(AF_INET, &(receiver_endpoint.sin_addr), address, INET_ADDRSTRLEN);
			cout << "Binding pada " << getAddress() << ":" << getPort() << " ..." << endl;
		}
	}

	/* RECEIVE DATA FROM TRANSMITTER */
	void doReceive(){
		socklen_t addrlen = sizeof(transmitter_endpoint);
		frame *c_frame = new frame;

		thread consume_t(doConsume, &rxbuf, &socket_, &transmitter_endpoint); // Create new thread for consuming the buffer data
		do{
			int recvlen = recvfrom(socket_, c_frame, sizeof(c_frame), 0, (struct sockaddr *)&transmitter_endpoint, &addrlen);
			//if (){

			//}
		} while(true);
		consume_t.join(); // Join the buffer-consumer thread to this thread
	}

	/* CONSUME DATA IN BUFFER */
	static void doConsume(buffer *buf, int *sockfd, sockaddr_in *transmitter){

	}

	/* GETTER RECEIVER BOUND ADDRESS */
	string getAddress(){
		return address;
	}

	/* GETTER RECEIVER BOUND PORT */
	string getPort(){
		return port;
	}

	/* CLOSE SOCKET */
	void closeSocket(){
		close(socket_);
	}
	
private:
	int socket_; // Opened socket
	struct sockaddr_in receiver_endpoint; // Receiver endpoint
	struct sockaddr_in transmitter_endpoint; // Transmitter endpoint
	char address[INET_ADDRSTRLEN]; // Receiver bound address
	const char* port; // Receiver bound port
	buffer rxbuf; // Frame buffer
};





int main(int argc, char const *argv[]){
	try{
		if (argc < 2){ // Parameter checking
			throw "Usage: receiver <port>";
		} 
		
		receiver rx(argv[1]); // Create receiver object

	} catch (const char* msg) { // Exception handling
		cerr << msg << endl;
	} catch(std::exception& e){
		cerr << "Exception: " << e.what() << endl;  // Unhandled exception
	}

	return 0;
}