CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: html.o

html.o: html.cpp html.h
	$(CC) $(CC_FLAGS) -c html.cpp

clean:
	rm *.o 2> /dev/null || true
