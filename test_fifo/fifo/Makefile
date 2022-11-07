.PHONY:all
all: clientFifo serverFifo

clientFifo:clientFifo.cc
	g++ -o $@ $^ -std=c++11

serverFifo:serverFifo.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf clientFifo serverFifo .fifo