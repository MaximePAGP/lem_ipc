#include <sys/sem.h>

void    sem_lock(int sem_id) {
    struct  sembuf   op = {0, -1, 0};

    semop(sem_id, &op, 1);
}

void    sem_unlock(int sem_id) {
    struct  sembuf  op = {0, 1, 0};

    semop(sem_id, &op, 1);
}
