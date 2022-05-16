CFLAGS = -g
CC = gcc
OBJ = erq.o execute.o socket.o

erq:	$(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $@

*.o: erq.h defs.h ../config.h ../machine.h


clean:
	rm -f erq *.o

dist: 	clean
	(cd ..; tar cvfzh tw-erq.tar.gz tw-erq; cd tw-erq;)

	
