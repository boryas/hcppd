CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/src/hcppd/
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)
all: blog_server
test: parser_test

blog_server: blog_server.o
	g++ $(CC_FLAGS) blog_server.o socket.o fd.o options.o request.o response.o parser.o resource.o fs.o html.o daemon.o -o blogd

blog_server.o: servers/blog/blog.cpp servers/blog/blog.h resource.o socket.o fd.o request.o response.o parser.o fs.o html.o daemon.o options.o lib/server.h
	g++ $(CC_FLAGS) -c servers/blog/blog.cpp -o blog_server.o

resource.o: servers/blog/resource.cpp servers/blog/resource.h
	g++ $(CC_FLAGS) -c servers/blog/resource.cpp -o resource.o

fs.o: lib/fs.cpp lib/fs.h
	g++ $(CC_FLAGS) -c lib/fs.cpp

html.o: lib/html.cpp lib/html.h
	g++ $(CC_FLAGS) -c lib/html.cpp

parser_test: parser_test.o
	g++ $(CC_FLAGS) parser_test.o parser.o request.o unit_test.o -o parser_test

parser_test.o: lib/http/parse/test/parser_test.cpp parser.o request.o unit_test.o
	g++ $(CC_FLAGS) -c lib/http/parse/test/parser_test.cpp

parser.o: lib/http/parse/parser.cpp lib/http/parse/parser.h
	g++ $(CC_FLAGS) -c lib/http/parse/parser.cpp

response.o: lib/http/response.cpp lib/http/response.h
	g++ $(CC_FLAGS) -c lib/http/response.cpp

request.o: lib/http/request.cpp lib/http/request.h
	g++ $(CC_FLAGS) -c lib/http/request.cpp

socket.o: lib/socket.cpp lib/socket.h fd.o
	g++ $(CC_FLAGS) -c lib/socket.cpp

fd.o: lib/fd.cpp lib/fd.h
	g++ $(CC_FLAGS) -c lib/fd.cpp

options.o: lib/options.cpp lib/options.h
	g++ $(CC_FLAGS) -c lib/options.cpp

daemon.o: lib/daemon.cpp lib/daemon.h
	g++ $(CC_FLAGS) -c lib/daemon.cpp

unit_test.o: lib/unit_test.cpp lib/unit_test.h
	g++ $(CC_FLAGS) -c lib/unit_test.cpp

clean:
	rm *.o blogd 2> /dev/null || true
