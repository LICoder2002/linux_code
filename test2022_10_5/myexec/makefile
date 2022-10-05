NY:all
all:myexec mycmd

myexec:myexec.c
	gcc -o $@ $^
mycmd:mycmd.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f myexec mycmd
.PHONY:all

