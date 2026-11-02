#ifndef PLAYER_H
#define PLAYER_H

#include "ipc.h"

void    move_up(t_ipc *ipc, t_player *player);
void    move_down(t_ipc *ipc, t_player *player);
void    move_left(t_ipc *ipc, t_player *player);
void    move_right(t_ipc *ipc, t_player *player);

bool    add_player_safe(t_ipc *ipc, int team_id);
void    remove_player_safe(t_ipc *ipc);
void    remove_player_unsafe(t_ipc *ipc);

#endif
