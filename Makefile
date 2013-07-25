CC_FLAGS = -std=c++0x -Wall -Werror
all: hcppd

hcppd: hcppd.o lex.yy.o http.tab.o
	g++ $(CC_FLAGS) hcppd.o server.o socket.o lex.yy.o http.tab.o -o hcppd

hcppd.o: hcppd.cpp server.o
	g++ $(CC_FLAGS) -c hcppd.cpp

server.o: server.cpp http.tab.cpp lex.yy.cpp server.h socket.o http_response.h
	g++ $(CC_FLAGS) -c server.cpp

socket.o: socket.cpp socket.h util.h
	g++ $(CC_FLAGS) -c socket.cpp

parser: http.tab.cpp lex.yy.cpp
	g++ $(CC_FLAGS) -o http_parser http.tab.cpp lex.yy.cpp

http.tab.o: http.tab.cpp
	g++ $(CC_FLAGS) -o $@ -c http.tab.cpp

lex.yy.o: lex.yy.cpp
	g++ $(CC_FLAGS) -o $@ -c lex.yy.cpp

http.tab.cpp: http.y http_request.h
	bison -v -d -o http.tab.cpp http.y

lex.yy.cpp: http.lex http.tab.cpp
	flex -o lex.yy.cpp http.lex

clean:
	rm *.o hcppd lex.yy.* http.tab.* http_parser 2> /dev/null || true
