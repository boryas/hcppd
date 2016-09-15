CC_VERSION = -std=c++11
CC_FLAGS = $(CC_VERSION) -Wall -Werror
all: hcppd directory_server

directory_server: directory_server.o
	g++ $(CC_FLAGS) directory_server.o socket.o -o directory_server

directory_server.o: directory_server.cpp socket.o lib/http/response.h
	g++ $(CC_FLAGS) -c directory_server.cpp

hcppd: hcppd.o
	g++ $(CC_FLAGS) hcppd.o server.o socket.o options.o request.o -o hcppd

hcppd.o: hcppd.cpp server.o options.o
	g++ $(CC_FLAGS) -c hcppd.cpp

server.o: server.cpp server.h socket.o request.o
	g++ $(CC_FLAGS) -c server.cpp

request.o: lib/http/request.cpp lib/http/request.h
	g++ $(CC_FLAGS) -c lib/http/request.cpp

socket.o: lib/socket.cpp lib/socket.h
	g++ $(CC_FLAGS) -c lib/socket.cpp

options.o: lib/options.cpp lib/options.h
	g++ $(CC_FLAGS) -c lib/options.cpp

clean:
	rm *.o hcppd directory_server http_parser 2> /dev/null || true
