CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CWD = $(shell pwd)
CC = g++
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

parser_test: parser_test.o
	$(CC) $(CC_FLAGS) parser_test.o parser.o request.o unit_test.o -o parser_test

parser_test.o: lib/http/parse/test/parser_test.cpp parser.o request.o unit_test.o
	$(CC) $(CC_FLAGS) -c lib/http/parse/test/parser_test.cpp

parser.o: lib/http/parse/parser.cpp lib/http/parse/parser.h
	$(CC) $(CC_FLAGS) -c lib/http/parse/parser.cpp
