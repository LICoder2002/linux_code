.PHONY:all
all:clientTcp serverTcpd
Method=#-DMY_PROTOCOL

clientTcp:ClientTcp.cpp
	g++ -o $@ $^ $(Method) -std=c++11 -ljsoncpp

serverTcpd:ServerTcp.cpp
	g++ -o $@ $^ $(Method) -std=c++11 -ljsoncpp -lpthread

.PHONY:clean
clean:
	rm -f clientTcp serverTcpd