all: receiver transmitter

receiver:
	g++  -std=c++11 src/receiver.cpp src/frame.cpp src/crc32.cpp -o bin/receiver -lpthread

transmitter:
	g++ -std=c++11 src/transmitter.cpp src/frame.cpp src/crc32.cpp -o bin/transmitter -lpthread


# all: receiver transmitter

# receiver:
# 	g++ -W -Wall -Wextra -pedantic -std=c++11 src/receiver.cpp -o bin/receiver -lpthread

# transmitter:
# 	g++ -W -Wall -Wextra -pedantic -std=c++11 src/transmitter.cpp -o bin/transmitter -lpthread
