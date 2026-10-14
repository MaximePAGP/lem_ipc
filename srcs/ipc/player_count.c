#include "../inc/ipc.h"
#include "../inc/utils.h"


size_t  get_player_count(t_ipc *ipc) {
    size_t count;

    sem_lock(ipc->sem_id);
    count = ipc->map->player_count;
    sem_unlock(ipc->sem_id);
    return (count);
}

void    increment_player_count(t_ipc *ipc) {
    sem_lock(ipc->sem_id);
    ipc->map->player_count ++;
    sem_unlock(ipc->sem_id);
}

void    decrement_player_count(t_ipc *ipc) {
    sem_lock(ipc->sem_id);
    if (ipc->map->player_count > 0) {
        ipc->map->player_count --;
    }
    sem_unlock(ipc->sem_id);
}