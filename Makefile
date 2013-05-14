all: hcppd

hcppd: hcppd.o
	g++ hcppd.o server.o socket.o -o hcppd

hcppd.o: hcppd.cpp server.o
	g++ -c hcppd.cpp

server.o: server.cpp server.h socket.o
	g++ -c server.cpp

socket.o: socket.cpp socket.h util.h
	g++ -c socket.cpp

clean:
	rm *.o hcppd
