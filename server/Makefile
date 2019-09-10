server:	server.o	tp_socket.o	io.o
	gcc	-g	-pthread -o	server server.o	tp_socket.o	io.o

server.o:	server.c	tp_socket.h	io.h
	gcc	-g	-pthread -c	server.c

tp_socket.o:	tp_socket.c	tp_socket.h
	gcc	-g	-c	tp_socket.c

io.o:	io.c	io.h
	gcc	-g	-c	io.c

clean:
	rm	*.o
