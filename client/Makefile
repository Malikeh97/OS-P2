all: client

client: main.o  client.o client_socket.o util.o
	g++ main.o  client.o client_socket.o util.o -o client

client.o: client.cpp client.h client_socket.h util.h
	g++ -c client.cpp

main.o: main.cpp client.h client_socket.h util.h
	g++ -c main.cpp

client_socket.o: client_socket.cpp client_socket.h util.h network_exception.h
	g++ -c client_socket.cpp

util.o: util.cpp util.h
	g++ -c util.cpp

clean:
	rm *.o client

run:
	./client
