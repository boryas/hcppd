CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: socket.o

socket.o: socket.cpp socket.h $(PROJECT_ROOT_DIR)/lib/fs/fd.o
	$(CC) $(CC_FLAGS) -c socket.cpp

$(PROJECT_ROOT_DIR)/lib/fs/fd.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/fs fd.o

clean:
	rm *.o 2> /dev/null || true
