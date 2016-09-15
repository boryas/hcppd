CC_VERSION = -std=c++11
CC_FLAGS = $(CC_VERSION) -Wall -Werror
all: hcppd directory_server

directory_server: directory_server.o
	g++ $(CC_FLAGS) directory_server.o socket.o -o directory_server

directory_server.o: directory_server.cpp socket.o http_response.h
	g++ $(CC_FLAGS) -c directory_server.cpp

hcppd: hcppd.o lex.yy.o http.tab.o
	g++ $(CC_FLAGS) hcppd.o server.o socket.o lex.yy.o http.tab.o options.o -o hcppd

hcppd.o: hcppd.cpp server.o options.o
	g++ $(CC_FLAGS) -c hcppd.cpp

server.o: server.cpp http.tab.cpp lex.yy.cpp server.h socket.o
	g++ $(CC_FLAGS) -c server.cpp

socket.o: lib/socket.cpp lib/socket.h
	g++ $(CC_FLAGS) -c lib/socket.cpp

options.o: lib/options.cpp lib/options.h
	g++ $(CC_FLAGS) -c lib/options.cpp

parser: http.tab.cpp lex.yy.cpp
	g++ $(CC_VERSION) -o http_parser http.tab.cpp lex.yy.cpp

http.tab.o: http.tab.cpp
	g++ $(CC_FLAGS) -o $@ -c http.tab.cpp

lex.yy.o: lex.yy.cpp
	g++ $(CC_VERSION) -o $@ -c lex.yy.cpp

http.tab.cpp: http.y http_request.h
	bison -v -d -o http.tab.cpp http.y

lex.yy.cpp: http.lex http.tab.cpp
	flex -o lex.yy.cpp http.lex

clean:
	rm *.o hcppd directory_server lex.yy.* http.tab.* http_parser 2> /dev/null || true
