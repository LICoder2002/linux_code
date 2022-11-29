CC=g++
STD=-std=c++11
LD=-lpthread
bin=threadPool
src=ThreadPoolTest.cc

$(bin):$(src)
	$(CC) -o $@ $^ $(LD) $(STD)

.PHONY:clean
clean:
	rm -rf $(bin)