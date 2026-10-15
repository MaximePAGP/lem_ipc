#include "../../../inc/ipc.h"
#include "../../../inc/player.h"
#include "../../../inc/utils.h"
#include <string.h>



// static  bool    is_cell_safe() {

// }


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

    if (!can_add_player_unsafe(ipc, team_id)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        return ;
    }

    size_t  x;
    size_t  y;

    if (!find_spawn(ipc, &x, &y)) {
        sem_unlock(ipc->sem_id);
        G_IS_RUNNING = 0;
        print_msg(STDOUT_FILENO, "No available spawn position.\n");
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
