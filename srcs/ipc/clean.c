#include "../inc/ipc.h"


void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);


void clean_ipc(t_ipc *ipc) {
    sem_lock(ipc->sem_id);

    ipc->map->player_count --;
    
    int is_last = (ipc->map->player_count == 0);

   shmdt(ipc->map);

    sem_unlock(ipc->sem_id);
    if (!is_last) {
        return;
    }

    shmctl(ipc->shm_id, IPC_RMID, NULL);
    semctl(ipc->sem_id, 0, IPC_RMID);
    msgctl(ipc->msg_id, IPC_RMID, NULL);
}