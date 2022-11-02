
.PHONY:all
all: libmylib.so libmylib.a

#生成动态库
libmylib.so:test1.o test2.o
		gcc -shared -o libmylib.so test1.o test2.o

test1.o:test1.c
		gcc -fPIC -c test1.c -o test1.o -std=c99
test2.o:test2.c
		gcc -fPIC -c test2.c -o test2.o -std=c99


#生成静态库
libmylib.a:test1_s.o test2_s.o
	ar -rc libmylib.a test1_s.o test2_s.o
test1_s.o:test1.c
		gcc -c test1.c -o test1_s.o -std=c99

test2_s.o:test2.c
		gcc -c test2.c -o test2_s.o -std=c99

.PHONY:lib 
lib:
		mkdir -p lib-static/lib 
		mkdir -p lib-static/include
		cp *.a lib-static/lib 
		cp *.h lib-static/include
		mkdir -p lib-dyl/lib 
		mkdir -p lib-dyl/include 
		cp *.so lib-dyl/lib 
		cp *.h lib-dyl/include

.PHONY:clean
clean:
		rm -rf *.o *.a *.so lib-static lib-dyl
