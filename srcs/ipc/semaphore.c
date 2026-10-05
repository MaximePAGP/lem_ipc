#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void    sem_lock(int sem_id) {
    struct  sembuf   op = {0, -1, 0};

    while (semop(sem_id, &op, 1) == -1) {
        if (errno != EINTR) {
            write(STDERR_FILENO, "semop failed\n", 14);
            exit(EXIT_FAILURE);
        }
    }
}

void    sem_unlock(int sem_id) {
    struct  sembuf  op = {0, 1, 0};

    while (semop(sem_id, &op, 1) == -1) {
        if (errno != EINTR) {
            write(STDERR_FILENO, "semop failed\n", 14);
            exit(EXIT_FAILURE);
        }
    }
}
