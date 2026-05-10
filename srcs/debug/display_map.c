#include "../../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);

static size_t trim_pid(size_t pid) {
    if (pid < 1000)
        return (pid);
    return (pid % 1000);
}

static void apply_color(size_t team_id, size_t pid) {
    size_t color = (16 + (team_id % 216));

    printf("\x1b[38;5;%zum%3zu\x1b[0m", color, trim_pid(pid));
}

static int find_player_index(t_ipc *ipc, pid_t pid) {
    for (size_t i = 0; i < PLAYER_MAX_LIMIT; i++) {
        if (ipc->players[i].pid == pid)
            return (i);
    }
    return (-1);
}

static t_player *find_player(t_ipc *ipc, pid_t pid) {
    int index = find_player_index(ipc, pid);

    if (index == -1)
        return (NULL);

    return (&ipc->players[index]);
}

void display_map(t_ipc *ipc) {
    sem_lock(ipc->sem_id);

    t_map *map = ipc->map;

    printf("\033[H\033[J");
    printf("My pid is %zu\n", trim_pid(getpid()));
    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++)
            printf("+-------");
        printf("+\n");

        for (size_t x = 0; x < MAP_WIDTH; x++) {
            size_t cell = map->cells[y][x];

            if (cell != 0) {
                t_player *player = find_player(ipc, (pid_t)cell);
                
                if (player) {
                    printf("|  ");
                    apply_color(player->team_id, trim_pid(cell));
                    printf("  ");
                } else
                    printf("| %zu ", trim_pid(cell));
            } else
                printf("|   %zu   ", cell);
        }
        printf("|\n");
    }

    for (size_t x = 0; x < MAP_WIDTH; x++)
        printf("+-------");
    printf("+\n\n");

    sem_unlock(ipc->sem_id);
    sleep(1);
}
