all: hcppd

hcppd: hcppd.o
	g++ hcppd.o server.o socket.o -o hcppd

hcppd.o: hcppd.cpp server.o
	g++ -c hcppd.cpp

server.o: server.cpp server.h socket.o
	g++ -c server.cpp

socket.o: socket.cpp socket.h util.h
	g++ -c socket.cpp

parser: http.lex http.y http.h
	bison -v -d -o http.tab.cpp http.y
	flex -o lex.yy.cpp http.lex
	g++ -o http_parser http.tab.cpp lex.yy.cpp

clean:
	rm *.o hcppd lex.yy.* http.tab.* http_parser
