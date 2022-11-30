.PHONY:all
all:udpClient udpServer

udpClient:UdpClient.cpp
	g++ -o $@ $^ -std=c++11 -lpthread
 
udpServer:UdpServer.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf udpClient udpServer