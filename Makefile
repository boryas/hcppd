CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)
all: hcppd

hcppd: hcppd.o
	g++ $(CC_FLAGS) hcppd.o blog_server.o socket.o options.o request.o response.o fs.o html.o -o hcppd

hcppd.o: hcppd.cpp blog_server.o http_fs_server.o options.o
	g++ $(CC_FLAGS) -c hcppd.cpp

blog_server.o: servers/blog.cpp servers/blog.h socket.o request.o response.o fs.o html.o lib/server.h
	g++ $(CC_FLAGS) -c servers/blog.cpp -o blog_server.o

http_fs_server.o: servers/http_fs.cpp servers/http_fs.h socket.o request.o response.o fs.o lib/server.h
	g++ $(CC_FLAGS) -c servers/http_fs.cpp -o http_fs_server.o

fs.o: lib/fs.cpp lib/fs.h
	g++ $(CC_FLAGS) -c lib/fs.cpp

html.o: lib/html.cpp lib/html.h
	g++ $(CC_FLAGS) -c lib/html.cpp

response.o: lib/http/response.cpp lib/http/response.h
	g++ $(CC_FLAGS) -c lib/http/response.cpp

request.o: lib/http/request.cpp lib/http/request.h
	g++ $(CC_FLAGS) -c lib/http/request.cpp

socket.o: lib/socket.cpp lib/socket.h
	g++ $(CC_FLAGS) -c lib/socket.cpp

options.o: lib/options.cpp lib/options.h
	g++ $(CC_FLAGS) -c lib/options.cpp

clean:
	rm *.o hcppd 2> /dev/null || true
