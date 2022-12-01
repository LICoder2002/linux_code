.PHONY:all
all:clientTcp serverTcp

clientTcp:ClientTcp.cpp
	g++ -o $@ $^ -std=c++11

serverTcp:ServerTcp.cpp
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f clientTcp serverTcp