#include "../inc/utils.h"
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void    sem_op(int sem_id, short int delta) {
    struct  sembuf  op = {0, delta, SEM_UNDO};

    while (semop(sem_id, &op, 1) == -1) {
        if (errno != EINTR) {
            print_error_system("semaphore.c: semop");
            exit(EXIT_FAILURE);
        }
    }
}

void    sem_lock(int sem_id) {
    sem_op(sem_id, -1);
}

void    sem_unlock(int sem_id) {
    sem_op(sem_id, 1);
}
