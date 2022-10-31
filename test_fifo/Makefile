.PHONY:all
all: clientFifo serverFifo

clientFifo:clientFifo.cpp
	g++ -Wall -o $@ $^ -std=c++11

serverFifo:serverFifo.cpp
	g++ -Wall -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf clientFifo serverFifo .fifo