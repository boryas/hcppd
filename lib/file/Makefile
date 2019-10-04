CC = clang++
CC_VERSION = -std=c++1y
CC_FLAGS = $(CC_VERSION)

all: fd.o

fd.o: Fd.cpp Fd.h
	$(CC) $(CC_FLAGS) -c fd.cpp -o fd.o

clean:
	rm *.o 2> /dev/null || true
