#include "ipc.h"

#include <stdlib.h>

bool    g_has_running = true;


void    init_signals(void);
int     handle_args(int argc, char **argv);
void    init_ipc(t_ipc *ipc);
void    add_player(t_ipc *ipc, int team_id);
void    display_map(t_ipc *ipc);
void    remove_player(t_ipc *ipc);
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
    init_signals();
    team_id = handle_args(argc, argv);
    init_ipc(&ipc);
    add_player(&ipc, team_id);
    while (g_has_running) {
        display_map(&ipc);
    }
    remove_player(&ipc);
    clean_ipc(&ipc);
    return (EXIT_SUCCESS);
}
