CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: daemon.o options.o

daemon.o: daemon.cpp daemon.h
	$(CC) $(CC_FLAGS) -c daemon.cpp

options.o: options.cpp options.h
	$(CC) $(CC_FLAGS) -c options.cpp

clean:
	rm *.o 2> /dev/null || true
