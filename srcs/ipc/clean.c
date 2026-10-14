#include "../inc/ipc.h"


void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);


void    clean_ipc(t_ipc *ipc) {
    bool    is_last;

    sem_lock(ipc->sem_id);
    ipc->map->player_count --;
    is_last = (ipc->map->player_count == 0);
    shmdt(ipc->map);
    ipc->map = NULL;

    if (!is_last) {
        sem_unlock(ipc->sem_id);
        return;
    }

    shmctl(ipc->shm_id, IPC_RMID, NULL);
    msgctl(ipc->msg_id, IPC_RMID, NULL);
    semctl(ipc->sem_id, 0, IPC_RMID);
}
