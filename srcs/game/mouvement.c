#include "../inc/ipc.h"

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);

static bool is_valid_move(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

static  bool is_cell_empty(t_ipc *ipc, int x, int y) {
    return (ipc->map->cells[y][x] == 0);
}

static void move_player(t_ipc *ipc, t_player *player, int dx, int dy) {
    sem_lock(ipc->sem_id);

    int new_x = (int)player->x + dx;
    int new_y = (int)player->y + dy;

    if (!is_valid_move(new_x, new_y) || !is_cell_empty(ipc, new_x, new_y)) {
        sem_unlock(ipc->sem_id);
        return ;
    }

    ipc->map->cells[player->y][player->x] = 0;

    player->x = new_x;
    player->y = new_y;

    ipc->map->cells[new_y][new_x] = player->pid;

    sem_unlock(ipc->sem_id);
}

void    move_up(t_ipc *ipc, t_player *player) {
    move_player(ipc, player, 0, -1);
}

void    move_down(t_ipc *ipc, t_player *player) {
    move_player(ipc, player, 0, 1);
}

void    move_left(t_ipc *ipc, t_player *player) {
    move_player(ipc, player, -1, 0);
}

void    move_right(t_ipc *ipc, t_player *player) {
    move_player(ipc, player, 1, 0);
}