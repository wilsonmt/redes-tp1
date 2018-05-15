functions: functions.o cliente.o servidor.o
		gcc functions.o cliente.o -o cliente
		gcc functions.o servidor.o -o servidor
cliente.o: cliente.c functions.h
		gcc -g -c cliente.c
servidor.o: servidor.c functions.h
		gcc -g -c servidor.c
functions.o: functions.h functions.c
		gcc -g -c functions.c
clean:
		rm *.o
		rm cliente
		rm servidor     