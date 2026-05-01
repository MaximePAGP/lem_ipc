#include "../inc/ipc.h"

#include <string.h>

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);
void    increment_player_count(t_ipc *ipc);

static  void    cleanup_on_error(t_ipc *ipc, bool is_first) {
    if (ipc->map && ipc->map != (void *)-1)
        shmdt(ipc->map);
    if (!is_first)
        return ;
    if (!ipc->shm_id)
        shmctl(ipc->shm_id, IPC_RMID, NULL);
    if (!ipc->sem_id)
        semctl(ipc->sem_id, 0, IPC_RMID);
    if (!ipc->msg_id)
        msgctl(ipc->msg_id, IPC_RMID, NULL);
}

void init_ipc(t_ipc *ipc) {
    ipc->shm_key = ftok("/tmp", 'S');
    ipc->sem_key = ftok("/tmp", 'M');
    ipc->msg_key = ftok("/tmp", 'Q');

    ipc->shm_id = shmget(ipc->shm_key, sizeof(t_map), IPC_CREAT | IPC_EXCL | 0666);

    if (ipc->shm_id != -1) {
        ipc->sem_id = semget(ipc->sem_key, 1, IPC_CREAT | IPC_EXCL | 0666);
        ipc->msg_id = msgget(ipc->msg_key, IPC_CREAT | IPC_EXCL | 0666);

        if (ipc->sem_id == -1 || ipc->msg_id == -1) {
            perror("Failed to create IPC resources");
            cleanup_on_error(ipc, true);
            exit(EXIT_FAILURE);
        }

        semctl(ipc->sem_id, 0, SETVAL, 1);

        ipc->map = shmat(ipc->shm_id, NULL, 0);
        if (ipc->map == (void *)-1) {
            perror("Failed to attach shared memory");
            cleanup_on_error(ipc, true);
            exit(EXIT_FAILURE);
        }

        memset(ipc->map, 0, sizeof(t_map));
        ipc->map->player_count++;

    } else {
        ipc->shm_id = shmget(ipc->shm_key, sizeof(t_map), 0666);
        ipc->sem_id = semget(ipc->sem_key, 1, 0666);
        ipc->msg_id = msgget(ipc->msg_key, 0666);

        if (!ipc->shm_id || !ipc->sem_id || !ipc->msg_id) {
            perror("Failed to get IPC resources");
            cleanup_on_error(ipc, false);
            exit(EXIT_FAILURE);
        }

        ipc->map = shmat(ipc->shm_id, NULL, 0);
        if (ipc->map == (void *)-1) {
            perror("Failed to attach shared memory");
            cleanup_on_error(ipc, false);
            exit(EXIT_FAILURE);
        }

        increment_player_count(ipc);
    }
}
