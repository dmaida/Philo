// Daniel Maida
// CS 360

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <math.h>
#define NUM_PHILO 5

int chopsticks;

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

void EatOrThink(int i){
    srand(i); // seed the amount time each Philosopher eats and thinks
    int time_eating = 0;
    int time_thinking = 0;

    while(time_eating < 100){

        int time_to_think = randomGaussian(11, 7);
        if (time_to_think < 0){
            time_to_think = 0;
        }
        printf("Philosopher %i thinking for %i seconds (total %i)\n",
            i,time_to_think, time_thinking);
        sleep(time_to_think);
        time_thinking += time_to_think;

        int left = i;
        int right = (i + 1) % NUM_PHILO;

        printf("Philosopher %i checking for chopsticks %i and %i\n", i,left,right);

        struct sembuf take[2];
        take[0].sem_num = left;
        take[0].sem_op = -1;
        take[0].sem_flg = 0;
        take[1].sem_num = right;
        take[1].sem_op = -1;
        take[1].sem_flg = 0;

        struct sembuf drop[2];
        drop[0].sem_num = left;
        drop[0].sem_op = 1;
        drop[0].sem_flg = 0;
        drop[1].sem_num = right;
        drop[1].sem_op = 1;
        drop[1].sem_flg = 0;

        int time_to_eat = 0;
        int errCheck = 0;

        if ((errCheck = semop(chopsticks, take, 2)) == 0) {
        	time_to_eat = randomGaussian(9, 3);
            if (time_to_eat < 0){
                time_to_eat = 0;
            }
            printf("Philosopher %i eating for %i seconds (total %i)\n", i, time_to_eat, time_eating);
            sleep(time_to_eat);
            time_eating += time_to_eat;
            semop(chopsticks, drop, 2);
        }
        if (errCheck == -1) {
            fprintf(stderr, "%s\n", "Semop returned an error");
        }
    }
    printf("Philosopher %i ate for a total of %i seconds and thought for %i seconds\n", i, time_eating, time_thinking);
}

void philo(){

    pid_t pid[NUM_PHILO];

    chopsticks = semget(IPC_PRIVATE, NUM_PHILO, IPC_CREAT | 0666);

    for(int i=0; i< NUM_PHILO; i++){ // set the values of the semaphore set
        semctl(chopsticks, i, SETVAL, 1);
    }

    for (int i = 0; i < NUM_PHILO; ++i){
        pid_t childID = fork();
        pid[i] = childID;
        if (pid[i] < 0){
            printf("Error\n");
            exit(1);
        }else if (pid[i] == 0){
            EatOrThink(i);
            printf("Philosopher %i is leaving the table\n", i);
            exit(1);
        }
    }
    for (int i = 0; i < NUM_PHILO; ++i){
        wait(NULL);
    }
    if (semctl(chopsticks, NUM_PHILO, IPC_RMID) < 0) {
        fprintf(stderr, "%s\n", "Error in semctl");
    }
}

int main(int argc, char* argv[]){
    philo();
    return 0;
}
