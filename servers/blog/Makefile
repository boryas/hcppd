CC = clang++
CC_VERSION = -std=c++1y
SSFS_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(SSFS_ROOT_DIR)

all: blog_server

blog_server: blog_server.o
	$(CC) $(CC_FLAGS) blog_server.o $(SSFS_ROOT_DIR)/lib/net/socket.o $(SSFS_ROOT_DIR)/lib/fs/fd.o $(SSFS_ROOT_DIR)/lib/run/options.o $(SSFS_ROOT_DIR)/lib/http/request.o $(SSFS_ROOT_DIR)/lib/http/response.o $(SSFS_ROOT_DIR)/lib/http/parse/parser.o resource.o $(SSFS_ROOT_DIR)/lib/fs/fs.o $(SSFS_ROOT_DIR)/lib/html/template.o $(SSFS_ROOT_DIR)/lib/run/daemon.o $(SSFS_ROOT_DIR)/lib/string/view.o -o blogd

blog_server.o: blog.cpp blog.h resource.o $(SSFS_ROOT_DIR)/lib/net/socket.o $(SSFS_ROOT_DIR)/lib/fs/fd.o $(SSFS_ROOT_DIR)/lib/http/request.o $(SSFS_ROOT_DIR)/lib/http/response.o $(SSFS_ROOT_DIR)/lib/http/parse/parser.o $(SSFS_ROOT_DIR)/lib/fs/fs.o $(SSFS_ROOT_DIR)/lib/html/template.o $(SSFS_ROOT_DIR)/lib/run/daemon.o $(SSFS_ROOT_DIR)/lib/run/options.o $(SSFS_ROOT_DIR)/lib/server/blocking.h $(SSFS_ROOT_DIR)/lib/string/view.o
	$(CC) $(CC_FLAGS) -c blog.cpp -o blog_server.o

resource.o: resource.cpp resource.h
	$(CC) $(CC_FLAGS) -c resource.cpp -o resource.o

$(SSFS_ROOT_DIR)/lib/net/socket.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/net socket.o

$(SSFS_ROOT_DIR)/lib/fs/fd.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/fs fd.o

$(SSFS_ROOT_DIR)/lib/fs/fs.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/fs fs.o

$(SSFS_ROOT_DIR)/lib/html/template.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/html template.o

$(SSFS_ROOT_DIR)/lib/run/options.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/run options.o

$(SSFS_ROOT_DIR)/lib/run/daemon.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/run daemon.o

$(SSFS_ROOT_DIR)/lib/http/request.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/http request.o

$(SSFS_ROOT_DIR)/lib/http/response.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/http response.o

$(SSFS_ROOT_DIR)/lib/http/parse/parser.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/http/parse parser.o

$(SSFS_ROOT_DIR)/lib/string/view.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/string view.o

clean:
	rm *.o blogd 2> /dev/null || true
