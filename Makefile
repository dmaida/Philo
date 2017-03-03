
CFLAGS= -std=c99 -Wall -pedantic

philo: philo.o
	gcc -g philo.o -lm -o philo

philo.o: philo.c
	gcc -g -c $(CFLAGS) philo.c

clean:
	rm *.o philo
