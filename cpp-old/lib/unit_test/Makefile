CC = clang++
CC_VERSION = -std=c++1y
SSFS_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(SSFS_ROOT_DIR)

all: unit_test.o

unit_test.o: unit_test.cpp unit_test.h
	$(CC) $(CC_FLAGS) -c unit_test.cpp

clean:
	rm *.o 2> /dev/null || true
