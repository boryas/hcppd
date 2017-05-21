CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs/
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: socket.o fd.o options.o daemon.o unit_test.o fs.o html.o

socket.o: socket.cpp socket.h fd.o
	$(CC) $(CC_FLAGS) -c socket.cpp

fd.o: fd.cpp fd.h
	$(CC) $(CC_FLAGS) -c fd.cpp

options.o: options.cpp options.h
	$(CC) $(CC_FLAGS) -c options.cpp

daemon.o: daemon.cpp daemon.h
	$(CC) $(CC_FLAGS) -c daemon.cpp

unit_test.o: unit_test.cpp unit_test.h
	$(CC) $(CC_FLAGS) -c unit_test.cpp

fs.o: fs.cpp fs.h
	$(CC) $(CC_FLAGS) -c fs.cpp

html.o: html.cpp html.h
	$(CC) $(CC_FLAGS) -c html.cpp

clean:
	rm *.o 2> /dev/null || true
