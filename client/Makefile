client: client.o tp_socket.o io.o
 gcc -g -lm -o client client.o tp_socket.o io.o

client.o: client.c tp_socket.h io.h
 gcc -g -c client.c -lm

tp_socket.o: tp_socket.c tp_socket.h
 gcc -g -c tp_socket.c

io.o: io.c io.h
 gcc -g -c io.c

clean:
 rm *.o
