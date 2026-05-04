#include "../../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);


int find_player_index(t_ipc *ipc, pid_t pid) {
    sem_lock(ipc->sem_id);
    for (size_t i = 0; i < PLAYER_LIMIT; i++) {
        if (ipc->players[i].pid == pid) {
            sem_unlock(ipc->sem_id);
            return (i);
        }
    }
    sem_unlock(ipc->sem_id);
    return (-1);
}

t_player    *get_player_ptr(t_ipc *ipc, pid_t pid) {
    int index = find_player_index(ipc, pid);

    if (index == -1)
        return (NULL);

    return (&ipc->players[index]);
}

int add_player(t_ipc *ipc, int team_id) {
    if (team_id < 0 || team_id >= TEAM_LIMIT)
        return (-1);

    sem_lock(ipc->sem_id);

    for (size_t i = 0; i < PLAYER_LIMIT; i++) {
        if (ipc->players[i].pid == 0) {
            ipc->players[i].pid = getpid();
            ipc->players[i].team_id = team_id;
            ipc->players[i].index = i;
            ipc->players[i].x = 0;
            ipc->players[i].y = 0;

            t_team *team = &ipc->teams[team_id];
            team->player_ids[team->count++] = i;

            sem_unlock(ipc->sem_id);
            return (i);
        }
    }

    sem_unlock(ipc->sem_id);
    return (-1);
}

void    remove_player(t_ipc *ipc, pid_t pid) {
    sem_lock(ipc->sem_id);

    int index = find_player_index(ipc, pid);
    if (index == -1) {
        sem_unlock(ipc->sem_id);
        return;
    }

    size_t team_id = ipc->players[index].team_id;
    t_team *team = &ipc->teams[team_id];

    for (size_t i = 0; i < team->count; i++) {
        if (team->player_ids[i] == (size_t)index)
        {
            team->player_ids[i] = team->player_ids[team->count - 1];
            team->count --;
            break;
        }
    }

    ipc->players[index].pid = 0;

    sem_unlock(ipc->sem_id);
}