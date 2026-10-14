#include "../inc/ipc.h"
#include "../inc/utils.h"

void    clean_ipc(t_ipc *ipc) {
    bool    is_last;

    sem_lock(ipc->sem_id);
    ipc->s_map->total_player --;
    is_last = (ipc->s_map->total_player == 0);
    shmdt(ipc->s_map);
    ipc->s_map = NULL;

    if (!is_last) {
        sem_unlock(ipc->sem_id);
        return;
    }

    shmctl(ipc->shm_id, IPC_RMID, NULL);
    msgctl(ipc->msg_id, IPC_RMID, NULL);
    semctl(ipc->sem_id, 0, IPC_RMID);
}
