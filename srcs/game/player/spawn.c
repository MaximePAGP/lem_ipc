#include "../../../inc/ipc.h"
#include "../../../inc/player.h"
#include "../../../inc/utils.h"
#include <string.h>



/*
 @details Check the 3x3 square centered on (x, y) and return true as soon as one cell (the center included) is occupied, whatever the team. Cells outside the map are ignored.
*/
static  bool    has_neighbor(t_ipc *ipc, size_t x, size_t y) {
    for (size_t h = 0; h < 3; h++) {
        for (size_t w = 0; w < 3; w++) {
            size_t  check_x = (x + w - 1);
            size_t  check_y = (y + h - 1);

            if (check_x >= MAP_WIDTH || check_y >= MAP_HEIGHT)
                continue ;

            if (ipc->s_map->cells[check_y][check_x] != EMPTY_CELL)
                return (true);
        }
    }
    return (false);
}




static size_t  count_spawns_unsafe(t_ipc *ipc) {
    size_t  count = 0;

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (!has_neighbor(ipc, x, y))
                count ++;
        }
    }
    return (count);
}


static bool find_spawn_unsafe(t_ipc *ipc, size_t *out_x, size_t *out_y) {
    size_t  count = count_spawns_unsafe(ipc);

    if (count == 0)
        return (false);

    size_t  k = random_range(count);

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (has_neighbor(ipc, x, y))
                continue ;
            if (k -- == 0) {
                *out_x = x;
                *out_y = y;
                return (true);
            }
        }
    }
    return (false);
}


bool    add_player_safe(t_ipc *ipc, int team_id) {
    sem_lock(ipc->sem_id);

    if (!can_add_player_unsafe(ipc, team_id)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        return (false);
    }

    size_t  x;
    size_t  y;

    if (!find_spawn_unsafe(ipc, &x, &y)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        print_msg(STDERR_FILENO, "No available spawn position.\n");
        return (false);
    }

    ipc->player.pid     = getpid();
    ipc->player.team_id = team_id;
    ipc->player.x       = x;
    ipc->player.y       = y;

    ipc->s_map->cells[y][x] = team_id;
    ipc->s_map->teams[team_id]++;

    sem_unlock(ipc->sem_id);
    return (true);
}



void    remove_player_safe(t_ipc *ipc) {
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
