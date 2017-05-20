CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CWD = $(shell pwd)
CC = g++
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

response.o: lib/http/response.cpp lib/http/response.h
	$(CC) $(CC_FLAGS) -c lib/http/response.cpp

request.o: lib/http/request.cpp lib/http/request.h
	$(CC) $(CC_FLAGS) -c lib/http/request.cpp
