CC = clang++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: view.o
test: view_test

view_test: view_test.o
	$(CC) $(CC_FLAGS) view_test.o view.o $(PROJECT_ROOT_DIR)/lib/unit_test/unit_test.o -o view_test

view_test.o: test/view_test.cpp view.o $(PROJECT_ROOT_DIR)/lib/unit_test/unit_test.o
	$(CC) $(CC_FLAGS) -c test/view_test.cpp

view.o: view.cpp view.h
	$(CC) $(CC_FLAGS) -c view.cpp

$(PROJECT_ROOT_DIR)/lib/unit_test.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/unit_test unit_test.o

clean:
	rm *.o 2> /dev/null || true
