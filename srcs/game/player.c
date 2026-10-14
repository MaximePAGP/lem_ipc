#include "../../inc/ipc.h"
#include "../inc/utils.h"


static bool has_limit_players_reached(t_ipc *ipc) {
    size_t count = 0;
    for (size_t i = 1; i <= TEAM_LIMIT; i++)
        count += ipc->s_map->teams[i];
    return count >= PLAYER_MAX_LIMIT;
}


static bool find_spawn(t_ipc *ipc, size_t *out_x, size_t *out_y) {
    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (ipc->s_map->cells[y][x] == EMPTY_CELL) {
                *out_x = x;
                *out_y = y;
                return (true);
            }
        }
    }
    return (false);
}


void    add_player(t_ipc *ipc, int team_id) {
    sem_lock(ipc->sem_id);

    if (has_limit_players_reached(ipc)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        return ;
    }

    if (ipc->s_map->teams[team_id] >= PLAYER_LIMIT) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        return ;
    }

    size_t  x;
    size_t  y;

    if (!find_spawn(ipc, &x, &y)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        return ;
    }

    ipc->player.pid     = getpid();
    ipc->player.team_id = team_id;
    ipc->player.x       = x;
    ipc->player.y       = y;

    ipc->s_map->cells[y][x] = team_id;
    ipc->s_map->teams[team_id]++;

    sem_unlock(ipc->sem_id);
}



void    remove_player(t_ipc *ipc) {
    t_player *player = &ipc->player;

    if (player->pid == 0)
        return ;

    sem_lock(ipc->sem_id);

    if (player->x < MAP_WIDTH && player->y < MAP_HEIGHT)
        ipc->s_map->cells[player->y][player->x] = EMPTY_CELL;

    if (ipc->s_map->teams[player->team_id] > 0)
        ipc->s_map->teams[player->team_id]--;

    player->pid = 0;

    sem_unlock(ipc->sem_id);
}
