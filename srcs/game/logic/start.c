#include "../../../inc/ipc.h"
#include "../../../inc/game_logic.h"
#include "../../../inc/utils.h"


bool    has_two_full_teams_unsafe(t_ipc *ipc) {
    t_team_count    teams[MAX_TEAMS] = {0};
    size_t          nb_teams = count_teams_unsafe(ipc, teams);

    return (count_full_teams(teams, nb_teams) >= 2);
}


void    wait_for_game_start_safe(t_ipc *ipc) {
    useconds_t  sleep_time_us = (300 * 1000);
    bool        is_started = false;

    print_msg(STDOUT_FILENO, "Waiting for players...\n");
    while (G_IS_RUNNING) {
        sem_lock(ipc->sem_id);
        if (!ipc->s_map->is_game_started && has_two_full_teams_unsafe(ipc))
            ipc->s_map->is_game_started = true;
        is_started = ipc->s_map->is_game_started;
        sem_unlock(ipc->sem_id);

        if (is_started)
            return ;
        usleep(sleep_time_us);
    }
}
