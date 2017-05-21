CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: template.o

template.o: template.cpp template.h $(PROJECT_ROOT_DIR)/lib/fs/fs.o
	$(CC) $(CC_FLAGS) -c template.cpp

$(PROJECT_ROOT_DIR)/lib/fs/fs.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/fs fs.o

uri_map.o: uri_map.cpp uri_map.h template.o
	$(CC) $(CC_FLAGS) -c uri_map.cpp

clean:
	rm *.o 2> /dev/null || true
