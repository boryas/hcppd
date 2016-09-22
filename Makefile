CC_VERSION = -std=c++11
CC_FLAGS = $(CC_VERSION) -Wall -Werror
all: hcppd

hcppd: hcppd.o
	g++ $(CC_FLAGS) hcppd.o server.o socket.o options.o request.o response.o fs.o -o hcppd

hcppd.o: hcppd.cpp server.o options.o
	g++ $(CC_FLAGS) -c hcppd.cpp

server.o: server.cpp server.h socket.o request.o response.o fs.o lib/server.h
	g++ $(CC_FLAGS) -c server.cpp

fs.o: lib/fs.cpp lib/fs.h
	g++ $(CC_FLAGS) -c lib/fs.cpp

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
