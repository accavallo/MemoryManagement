all: oss clean

CC = gcc -g

oss: oss.o
	$(CC) -o oss oss.o

oss.o: oss.c Proj6.h
	$(CC) -c oss.c

clean:
	rm -f *.o
