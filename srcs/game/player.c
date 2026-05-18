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


t_player    *get_player_ptr(t_ipc *ipc, pid_t pid) {
    int index = find_player_index(ipc, pid);

    if (index == -1)
        return (NULL);
    return (&ipc->players[index]);
}


static bool is_player_limit_reached(t_ipc *ipc) {
    size_t count = 0;
    for (size_t i = 0; i < PLAYER_MAX_LIMIT; i++) {
        if (ipc->players[i].pid != 0)
            count++;
    }
    return count >= PLAYER_MAX_LIMIT;
}


static bool find_spawn(t_ipc *ipc, size_t *out_x, size_t *out_y) {
    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (ipc->map->cells[y][x] == 0) {
                *out_x = x;
                *out_y = y;
                return (true);
            }
        }
    }
    return (false);
}


static int find_free_slot(t_ipc *ipc) {
    for (size_t i = 0; i < PLAYER_MAX_LIMIT; i++) {
        if (ipc->players[i].pid == 0) {
            printf("free slot check %zu: pid=%u\n", i, ipc->players[i].pid);

            return (i);
        }
    }
    return (-1);
}


void    add_player(t_ipc *ipc, int team_id) {
    if (team_id < 0 || team_id >= TEAM_LIMIT) {
        g_has_running = false;
        return ;
    }

    sem_lock(ipc->sem_id);

    if (is_player_limit_reached(ipc)) {
        sem_unlock(ipc->sem_id);
        g_has_running = false;
        return ;
    }

    t_teams *team = &ipc->teams[team_id];
    if (team->count >= PLAYER_LIMIT) {
        sem_unlock(ipc->sem_id);
        g_has_running = false;
        return ;
    }

    int slot = find_free_slot(ipc);
    if (slot == -1) {
        sem_unlock(ipc->sem_id);
        g_has_running = false;
        return ;
    }

    size_t  x;
    size_t  y;
    if (!find_spawn(ipc, &x, &y)) {
        sem_unlock(ipc->sem_id);
        g_has_running = false;
        return ;
    }

    ipc->players[slot].pid     = getpid();
    ipc->players[slot].team_id = team_id;
    ipc->players[slot].index   = slot;
    ipc->players[slot].x       = x;
    ipc->players[slot].y       = y;

    ipc->map->cells[y][x]           = getpid();
    team->player_ids[team->count++] = slot;

    sem_unlock(ipc->sem_id);
}



void    remove_player(t_ipc *ipc, pid_t pid) {
    sem_lock(ipc->sem_id);

    int index = find_player_index_unsafe(ipc, pid);
    if (index == -1) {
        sem_unlock(ipc->sem_id);
        return ;
    }

    size_t px = ipc->players[index].x;
    size_t py = ipc->players[index].y;
    if (px < MAP_WIDTH && py < MAP_HEIGHT)
        ipc->map->cells[py][px] = 0;

    size_t  team_id = ipc->players[index].team_id;
    t_teams *team   = &ipc->teams[team_id];

    for (size_t i = 0; i < team->count; i++) {
        if (team->player_ids[i] == (size_t)index) {
            team->player_ids[i] = team->player_ids[team->count - 1];
            team->count --;
            break ;
        }
    }

    ipc->players[index].pid = 0;

    sem_unlock(ipc->sem_id);
}


