/*
Daniel Maida
CS 360
Assignment 6
*/

#define _POSIX_SOURCE
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
#define PHILOSOPHERS_NUMB 5

/* successive calls to randomGaussian produce integer return values */
/* having a gaussian distribution with the given mean and standard  */
/* deviation.  Return values may be negative.                       */

void doChildStuff() {
	printf("%s\n", "Doing child stuff");
}


void philo() {
	int pids[PHILOSOPHERS_NUMB];

	for(int i = 0; i < PHILOSOPHERS_NUMB; i++) {
    pids[i] = fork();
    if(pids[i] < 0) {
        printf("Error");
        exit(1);
    } else if (pids[i] == 0) {
        printf("Child (%d): %d\n", i , getpid());
				//call something here
        exit(0);
    }
	}
	
	for (int i = 0; i < PHILOSOPHERS_NUMB; i++) {
		wait(NULL);
	}
}

int randomGaussian(int mean, int stddev) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
	if (rand() & (1 << 5))
	return (int) floor(mu + sigma * cos(f2) * f1);
	else
	return (int) floor(mu + sigma * sin(f2) * f1);
}

int main(int argc, char* argv[]){
	philo();
	return 0;
}
