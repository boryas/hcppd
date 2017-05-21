CC = g++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: blog_server

blog_server: blog_server.o
	$(CC) $(CC_FLAGS) blog_server.o $(PROJECT_ROOT_DIR)/lib/net/socket.o $(PROJECT_ROOT_DIR)/lib/fs/fd.o $(PROJECT_ROOT_DIR)/lib/run/options.o $(PROJECT_ROOT_DIR)/lib/http/request.o $(PROJECT_ROOT_DIR)/lib/http/response.o $(PROJECT_ROOT_DIR)/lib/http/parse/parser.o resource.o $(PROJECT_ROOT_DIR)/lib/fs/fs.o $(PROJECT_ROOT_DIR)/lib/html/html.o $(PROJECT_ROOT_DIR)/lib/run/daemon.o -o blogd

blog_server.o: blog.cpp blog.h resource.o $(PROJECT_ROOT_DIR)/lib/net/socket.o $(PROJECT_ROOT_DIR)/lib/fs/fd.o $(PROJECT_ROOT_DIR)/lib/http/request.o $(PROJECT_ROOT_DIR)/lib/http/response.o $(PROJECT_ROOT_DIR)/lib/http/parse/parser.o $(PROJECT_ROOT_DIR)/lib/fs/fs.o $(PROJECT_ROOT_DIR)/lib/html/html.o $(PROJECT_ROOT_DIR)/lib/run/daemon.o $(PROJECT_ROOT_DIR)/lib/run/options.o $(PROJECT_ROOT_DIR)/lib/server/blocking.h
	$(CC) $(CC_FLAGS) -c blog.cpp -o blog_server.o

resource.o: resource.cpp resource.h
	$(CC) $(CC_FLAGS) -c resource.cpp -o resource.o

$(PROJECT_ROOT_DIR)/lib/net/socket.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/net socket.o

$(PROJECT_ROOT_DIR)/lib/fd.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/fs fd.o

$(PROJECT_ROOT_DIR)/lib/fs.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/fs fs.o

$(PROJECT_ROOT_DIR)/lib/html.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/html html.o

$(PROJECT_ROOT_DIR)/lib/options.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/run options.o

$(PROJECT_ROOT_DIR)/lib/daemon.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/run daemon.o

$(PROJECT_ROOT_DIR)/lib/http/request.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/http request.o

$(PROJECT_ROOT_DIR)/lib/http/response.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/http response.o

$(PROJECT_ROOT_DIR)/lib/http/parse/parser.o:
	$(MAKE) -C $(PROJECT_ROOT_DIR)/lib/http/parse parser.o

clean:
	rm *.o blogd 2> /dev/null || true
