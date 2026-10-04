#include "../../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);

static size_t trim_pid(size_t pid) {
    if (pid < 1000)
        return (pid);
    return (pid % 1000);
}

static void apply_color(int team_id) {
    int color = (16 + (team_id % 216));

    printf("\x1b[38;5;%dm%3d\x1b[0m", color, team_id);
}

static bool is_me(t_ipc *ipc, size_t x, size_t y) {
    return (ipc->player.pid != 0 && ipc->player.x == x && ipc->player.y == y);
}

void display_map(t_ipc *ipc) {
    sem_lock(ipc->sem_id);

    t_map *map = ipc->map;

    printf("\033[H\033[J");
    printf("My pid is %zu, team %d\n", trim_pid(getpid()), ipc->player.team_id);
    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++)
            printf("+-------");
        printf("+\n");

        for (size_t x = 0; x < MAP_WIDTH; x++) {
            int cell = map->cells[y][x];

            if (cell != EMPTY_CELL) {
                printf("| %c", is_me(ipc, x, y) ? '*' : ' ');
                apply_color(cell);
                printf("  ");
            } else
                printf("|       ");
        }
        printf("|\n");
    }

    for (size_t x = 0; x < MAP_WIDTH; x++)
        printf("+-------");
    printf("+\n\n");

    sem_unlock(ipc->sem_id);
    sleep(1);
}
