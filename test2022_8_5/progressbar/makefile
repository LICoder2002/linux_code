progressbar:progressbar.o main.o
	g++ -o progressbar progressbar.o main.o
progressbar.o:progressbar.cpp 
	g++ -c progressbar.cpp 
main.o:main.cpp 
	g++ -c main.cpp -o main.o

.PHONY:clean
clean:
	rm -f *.o progressbar
