.PHONY:all
all:clientTcp serverTcpd

clientTcp:ClientTcp.cpp
	g++ -o $@ $^ -std=c++11

serverTcpd:ServerTcp.cpp
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f clientTcp serverTcpd