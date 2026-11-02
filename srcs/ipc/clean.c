#include "../inc/ipc.h"
#include "../inc/utils.h"

void    clean_ipc(t_ipc *ipc) {
    struct shmid_ds info;
    bool            is_last = false;

    sem_lock(ipc->sem_id);
    if (shmctl(ipc->shm_id, IPC_STAT, &info) != -1)
        is_last = (info.shm_nattch == 1);
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
