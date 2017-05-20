CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CWD = $(shell pwd)
CC = g++
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)
all: blog_server
test: parser_test

blog_server: blog_server.o
	$(CC) $(CC_FLAGS) blog_server.o socket.o fd.o options.o request.o response.o parser.o resource.o fs.o html.o daemon.o -o blogd

blog_server.o: servers/blog/blog.cpp servers/blog/blog.h resource.o socket.o fd.o request.o response.o parser.o fs.o html.o daemon.o options.o lib/server.h
	$(CC) $(CC_FLAGS) -c servers/blog/blog.cpp -o blog_server.o

resource.o: servers/blog/resource.cpp servers/blog/resource.h
	$(CC) $(CC_FLAGS) -c servers/blog/resource.cpp -o resource.o

include lib/Makefile
include lib/http/Makefile
include lib/http/parse/Makefile

clean:
	rm *.o blogd parser_test 2> /dev/null || true
