#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "ipc.h"

#define MAX_TEAMS (MAP_WIDTH * MAP_HEIGHT)

typedef struct  s_team_count {
    int     team_id;
    size_t  count;
}   t_team_count;

bool    is_dead_unsafe(t_ipc *ipc, t_player *player);
size_t  count_teams_unsafe(t_ipc *ipc, t_team_count teams[MAX_TEAMS]);
size_t  count_full_teams(const t_team_count *teams, size_t nb_teams);
bool    has_two_full_teams_unsafe(t_ipc *ipc);
bool    check_game_over_unsafe(t_ipc *ipc);
void    announce_end_status(int winner_team_id, int team_id);
void    wait_for_game_start_safe(t_ipc *ipc);

    #endif
