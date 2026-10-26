#include "../inc/ipc.h"
#include "../inc/utils.h"


size_t  get_player_count(t_ipc *ipc) {
    size_t count;

    sem_lock(ipc->sem_id);
    count = ipc->s_map->total_player;
    sem_unlock(ipc->sem_id);
    return (count);
}

void    increment_player_count(t_ipc *ipc) {
    sem_lock(ipc->sem_id);
    ipc->s_map->total_player ++;
    sem_unlock(ipc->sem_id);
}

void    decrement_player_count(t_ipc *ipc) {
    sem_lock(ipc->sem_id);
    if (ipc->s_map->total_player > 0) {
        ipc->s_map->total_player --;
    }
    sem_unlock(ipc->sem_id);
}
