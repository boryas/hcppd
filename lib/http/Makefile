CC = clang++
CC_VERSION = -std=c++1y
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)
PROJECT_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)

all: response.o request.o

response.o: response.cpp response.h
	$(CC) $(CC_FLAGS) -c response.cpp

request.o: request.cpp request.h
	$(CC) $(CC_FLAGS) -c request.cpp

clean:
	rm *.o 2> /dev/null || true
	$(MAKE) -C parse clean
