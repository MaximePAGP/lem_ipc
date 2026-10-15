#include "../../inc/ipc.h"
#include "../../inc/game_logic.h"
#include "../../inc/utils.h"


static size_t   count_players_unsafe(const t_map *map) {
    size_t count = 0;

    for (size_t i = 1; i <= TEAM_LIMIT; i++)
        count += map->teams[i];
    return (count);
}


static bool     is_board_full_unsafe(const t_map *map) {
    return (count_players_unsafe(map) >= PLAYER_MAX_LIMIT);
}


static bool     is_team_full_unsafe(const t_map *map, int team_id) {
    return (map->teams[team_id] >= PLAYER_LIMIT);
}


bool    can_add_player_unsafe(t_ipc *ipc, int team_id) {
    if (is_board_full_unsafe(ipc->s_map)) {
        print_msg(STDOUT_FILENO, "Maximum player limit reached. Cannot add more players.\n");
        return (false);
    }

    if (is_team_full_unsafe(ipc->s_map, team_id)) {
        print_msg(STDOUT_FILENO, "Player limit reached for this team.\n");
        return (false);
    }

    return (true);
}
