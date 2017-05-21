CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(CWD) -I$(PROJECT_ROOT_DIR)

all: parser.o
test: parser_test

parser_test: parser_test.o
	$(CC) $(CC_FLAGS) parser_test.o parser.o $(PROJECT_ROOT_DIR)/lib/http/request.o $(PROJECT_ROOT_DIR)/lib/unit_test/unit_test.o -o parser_test

parser_test.o: test/parser_test.cpp parser.o $(PROJECT_ROOT_DIR)/lib/http/request.o $(PROJECT_ROOT_DIR)/lib/unit_test/unit_test.o
	$(CC) $(CC_FLAGS) -c test/parser_test.cpp

parser.o: parser.cpp parser.h $(PROJECT_ROOT_DIR)/lib/http/request.o
	$(CC) $(CC_FLAGS) -c parser.cpp

$(PROJECT_ROOT_DIR)/lib/http/request.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/http request.o

$(PROJECT_ROOT_DIR)/lib/unit_test.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/unit_test unit_test.o

clean:
	rm parser_test *.o 2> /dev/null || true
