CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CWD = $(shell pwd)
CC = g++
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

socket.o: lib/socket.cpp lib/socket.h fd.o
	$(CC) $(CC_FLAGS) -c lib/socket.cpp

fd.o: lib/fd.cpp lib/fd.h
	$(CC) $(CC_FLAGS) -c lib/fd.cpp

options.o: lib/options.cpp lib/options.h
	$(CC) $(CC_FLAGS) -c lib/options.cpp

daemon.o: lib/daemon.cpp lib/daemon.h
	$(CC) $(CC_FLAGS) -c lib/daemon.cpp

unit_test.o: lib/unit_test.cpp lib/unit_test.h
	$(CC) $(CC_FLAGS) -c lib/unit_test.cpp

fs.o: lib/fs.cpp lib/fs.h
	$(CC) $(CC_FLAGS) -c lib/fs.cpp

html.o: lib/html.cpp lib/html.h
	$(CC) $(CC_FLAGS) -c lib/html.cpp
