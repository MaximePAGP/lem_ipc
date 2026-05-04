#include "../../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);
t_player*get_player_ptr(t_ipc *ipc, pid_t pid);

static  void    apply_color(size_t team, size_t player_id) {
    printf("\x1b[%zum%zu\x1b[0m\n", team, player_id);
}


static    int find_player_index(t_ipc *ipc, pid_t pid) {
    for (size_t i = 0; i < PLAYER_LIMIT; i++) {
        if (ipc->players[i].pid == pid) {
            return (i);
        }
    }
    return (-1);
}

static    t_player    *find_player(t_ipc *ipc, pid_t pid) {
    int index = find_player_index(ipc, pid);

    if (index == -1)
        return (NULL);

    return (&ipc->players[index]);
}


void    display_map(t_ipc *ipc) {
    sem_lock(ipc->sem_id);

    t_map map = ipc->map;

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++)
            printf("+-----");
        printf("+\n");
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (map.cells[y][x] != 0) {
                t_player *player = find_player(ipc, getpid());
                apply_color(player->team_id, player->pid);
            } else {
                printf("| %3zu ", map.cells[y][x]);
            }
        }
        printf("|\n");
    }
    for (size_t x = 0; x < MAP_WIDTH; x++)
        printf("+-----");
    printf("+\n");
    sem_unlock(ipc->sem_id);
}