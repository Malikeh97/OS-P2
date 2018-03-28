all: server

server: main.cpp server_socket.o server.o	util.o
	g++ main.cpp server_socket.o server.o util.o -o server

server.o: server.cpp server.h
	g++ -c server.cpp

server_socket.o: server_socket.cpp server_socket.h util.h network_exception.h
	g++ -c server_socket.cpp

util.o: util.cpp util.h
	g++ -c util.cpp

clean:
	rm *.o server

run:
	./server
