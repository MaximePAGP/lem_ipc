#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "ipc.h"

#define MAX_TEAMS (MAP_WIDTH * MAP_HEIGHT)

typedef struct  s_team_count {
    int     team_id;
    size_t  count;
}   t_team_count;

bool    is_dead_unsafe(t_ipc *ipc, t_player *player);
bool    has_two_full_teams_unsafe(t_ipc *ipc);
bool    is_last_team_unsafe(t_ipc *ipc);
void    wait_for_game_start_safe(t_ipc *ipc);

    #endif
