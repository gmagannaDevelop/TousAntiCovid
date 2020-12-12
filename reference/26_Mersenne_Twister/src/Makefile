CC = gcc
CFLAGS   = -O3 -lgsl -lgslcblas
COMMONSRC = libraries.h 
EXECUTABLE = ../bin/random_numbers

../bin/random_numbers: parsing.o dev_random.o\
                       mersenne_twister.o random_numbers.o
	$(CC) -o $(EXECUTABLE) parsing.o dev_random.o\
                               mersenne_twister.o random_numbers.o $(CFLAGS)


parsing.o: parsing.c parsing.h $(COMMONSRC)
	$(CC) -c parsing.c $(CFLAGS)

dev_random.o: dev_random.c dev_random.h $(COMMONSRC)
	$(CC) -c dev_random.c $(CFLAGS)

mersenne_twister.o: mersenne_twister.c mersenne_twister.h $(COMMONSRC)
	$(CC) -c mersenne_twister.c $(CFLAGS)

random_numbers.o: random_numbers.c random_numbers.h $(COMMONSRC)
	$(CC) -c random_numbers.c $(CFLAGS)


clean:
	rm -f *.o $(EXECUTABLE)
