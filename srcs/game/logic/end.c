#include "../../../inc/ipc.h"
#include "../../../inc/game_logic.h"
#include "../../../inc/utils.h"


static int  find_winner_team(const t_team_count *teams, size_t nb_teams) {
    int     winner_team_id = EMPTY_CELL;
    size_t  best_count = 0;

    for (size_t i = 0; i < nb_teams; i++) {
        if (teams[i].count > best_count) {
            best_count = teams[i].count;
            winner_team_id = teams[i].team_id;
        } else if (teams[i].count == best_count)
            winner_team_id = EMPTY_CELL;
    }

    return (winner_team_id);
}


bool    check_game_over_unsafe(t_ipc *ipc) {
    t_map           *map = ipc->s_map;
    t_team_count    teams[MAX_TEAMS] = {0};
    size_t          nb_teams;

    if (map->is_game_over)
        return (true);

    nb_teams = count_teams_unsafe(ipc, teams);
    if (count_full_teams(teams, nb_teams) >= 2)
        return (false);

    map->is_game_over = true;
    map->winner_team_id = find_winner_team(teams, nb_teams);
    return (true);
}


void    announce_end_status(int winner_team_id, int team_id) {
    if (winner_team_id == team_id)
        print_msg(STDOUT_FILENO, "You won!\n");
    else if (winner_team_id == EMPTY_CELL)
        print_msg(STDOUT_FILENO, "Draw!\n");
    else
        print_msg(STDOUT_FILENO, "You lost!\n");
}
