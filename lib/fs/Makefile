CC = clang++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: fd.o fs.o

fd.o: fd.cpp fd.h
	$(CC) $(CC_FLAGS) -c fd.cpp

fs.o: fs.cpp fs.h
	$(CC) $(CC_FLAGS) -c fs.cpp

clean:
	rm *.o 2> /dev/null || true
