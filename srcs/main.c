#include "ipc.h"
#include "game_logic.h"
#include "player.h"
#include "utils.h"

#include <stdlib.h>

volatile sig_atomic_t   G_IS_RUNNING = 1;


void    init_signals(void);
int     handle_args(int argc, char **argv);
void    init_ipc(t_ipc *ipc);
void    display_map_unsafe(t_ipc *ipc);
void    clean_ipc(t_ipc *ipc);

static void set_ipc_defaults(t_ipc *ipc) {
    ipc->shm_key = -1;
    ipc->sem_key = -1;
    ipc->msg_key = -1;
    ipc->shm_id = -1;
    ipc->sem_id = -1;
    ipc->msg_id = -1;
}


int main(int argc, char **argv) {
    t_ipc   ipc = {0};
    int     team_id;

    set_ipc_defaults(&ipc);
    team_id = handle_args(argc, argv);
    init_signals();
    init_random();
    init_ipc(&ipc);
    if (!add_player_safe(&ipc, team_id)) {
        clean_ipc(&ipc);
        return (EXIT_FAILURE);
    }
    // ipc.s_map->cells[ipc.player.y][ipc.player.x] = 0;
    // ipc.player.x = 3;
    // ipc.player.y = 3;
    // ipc.s_map->cells[ipc.player.y][ipc.player.x] = ipc.player.team_id;
    // ipc.s_map->cells[ipc.player.y][ipc.player.x+1] = 3;
    // ipc.s_map->cells[ipc.player.y+1][ipc.player.x] = 3;
    wait_for_game_start_safe(&ipc);
    while (G_IS_RUNNING) {
        sem_lock(ipc.sem_id);
        if (is_dead_unsafe(&ipc, &ipc.player)) {
            remove_player_unsafe(&ipc);
            sem_unlock(ipc.sem_id);
            print_msg(STDOUT_FILENO, "You were catched!\n");
            break;
        }
        if (is_last_team_unsafe(&ipc)) {
            sem_unlock(ipc.sem_id);
            print_msg(STDOUT_FILENO, "You won!\n");
            break;
        }
        display_map_unsafe(&ipc);
        sem_unlock(ipc.sem_id);
        sleep(1);
    }
    remove_player_safe(&ipc);
    clean_ipc(&ipc);
    return (EXIT_SUCCESS);
}
