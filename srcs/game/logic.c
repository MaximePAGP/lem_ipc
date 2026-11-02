#include "../../inc/ipc.h"
#include "../../inc/game_logic.h"
#include "../../inc/utils.h"


/*
 @details a player is dead when two players of the same enemy team are adjacent to him (left, right, up, down)
*/
bool    is_dead_unsafe(t_ipc *ipc, t_player *player) {
    t_map   *map = ipc->s_map;
    int     cells[4];
    size_t  count = 0;
    size_t  x = player->x;
    size_t  y = player->y;

    if (x > 0)
        cells[count++] = map->cells[y][x - 1];
    if ((x + 1) < MAP_WIDTH)
        cells[count++] = map->cells[y][x + 1];
    if (y > 0)
        cells[count++] = map->cells[y - 1][x];
    if ((y + 1) < MAP_HEIGHT)
        cells[count++] = map->cells[y + 1][x];

    for (size_t i = 0; i < count; i++) {
        if (cells[i] == EMPTY_CELL || cells[i] == player->team_id)
            continue ;
        for (size_t j = (i + 1); j < count; j++) {
            if (cells[i] == cells[j])
                return (true);
        }
    }

    return (false);
}
