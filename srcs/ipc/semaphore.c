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
            const char *msg = "semop failed\n";
            write(STDERR_FILENO, msg, strlen(msg));
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
