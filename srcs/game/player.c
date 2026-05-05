#include "../../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);


static int  find_player_index_unsafe(t_ipc *ipc, pid_t pid) {
    for (size_t i = 0; i < PLAYER_LIMIT; i++) {
        if (ipc->players[i].pid == pid)
            return (i);
    }
    return (-1);
}


int find_player_index(t_ipc *ipc, pid_t pid) {
    int index;

    sem_lock(ipc->sem_id);
    index = find_player_index_unsafe(ipc, pid);
    sem_unlock(ipc->sem_id);
    return (index);
}
t_player    *get_player_ptr(t_ipc *ipc, pid_t pid)
{
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
void assign_team(t_ipc *ipc, t_player *player, int team_id) {
    sem_lock(ipc->sem_id);

    size_t i;
    for (i = 0; i < PLAYER_MAX_LIMIT; i++) {
        if (ipc->players[i].pid == 0)
            break;
    }

    if (i == PLAYER_MAX_LIMIT) {
        sem_unlock(ipc->sem_id);
        return;
    }

    t_teams *team = &ipc->teams[team_id];

    if (team->count >= PLAYER_LIMIT) {
        sem_unlock(ipc->sem_id);
        return;
    }

    player->index = i;
    player->team_id = team_id;
    player->pid = getpid();

    player->x = 0;
    player->y = 0;

    ipc->players[i] = *player;
    ipc->map->cells[player->y][player->x] = player->pid;
    team->player_ids[team->count++] = i;

    sem_unlock(ipc->sem_id);
}
