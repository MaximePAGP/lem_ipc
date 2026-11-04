#include "../../../inc/ipc.h"
#include "../../../inc/game_logic.h"
#include "../../../inc/utils.h"


static t_team_count *find_team(t_team_count *teams, size_t nb_teams, int team_id) {
    for (size_t i = 0; i < nb_teams; i++) {
        if (teams[i].team_id == team_id)
            return (&teams[i]);
    }
    return (NULL);
}

static t_team_count *push_last(t_team_count *teams, size_t *nb_teams, int team_id) {
    t_team_count    *team = &teams[*nb_teams];

    team->team_id = team_id;
    team->count = 0;
    (*nb_teams)++;
    return (team);
}


static void add_player_to_team(t_team_count *teams, size_t *nb_teams, int team_id) {
    t_team_count    *team = find_team(teams, *nb_teams, team_id);

    if (!team)
        team = push_last(teams, nb_teams, team_id);
    team->count++;
}


size_t  count_teams_unsafe(t_ipc *ipc, t_team_count teams[MAX_TEAMS]) {
    t_map   *map = ipc->s_map;
    size_t  nb_teams = 0;

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            if (map->cells[y][x] != EMPTY_CELL)
                add_player_to_team(teams, &nb_teams, map->cells[y][x]);
        }
    }

    return (nb_teams);
}


size_t  count_full_teams(const t_team_count *teams, size_t nb_teams) {
    size_t  full_teams = 0;

    for (size_t i = 0; i < nb_teams; i++) {
        if (teams[i].count >= 2)
            full_teams++;
    }

    return (full_teams);
}
