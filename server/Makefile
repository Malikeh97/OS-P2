all: server

server: main.cpp server_socket.o server.o	util.o worker.o user.o
	g++ --std=c++11 main.cpp server_socket.o server.o util.o worker.o user.o -o server

server.o: server.cpp server.h
	g++ --std=c++11 -c server.cpp

server_socket.o: server_socket.cpp server_socket.h util.h network_exception.h
	g++ -c server_socket.cpp

worker.o: worker.cpp worker.h
	g++ --std=c++11 -c worker.cpp

user.o: user.cpp user.h
	g++ -c user.cpp

util.o: util.cpp util.h
	g++ -c util.cpp

clean:
	rm *.o server

run:
	./server
