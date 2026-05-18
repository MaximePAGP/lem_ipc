#ifndef IPC_H
#define IPC_H


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

    #ifndef PLAYER_MAX_LIMIT
        #define PLAYER_MAX_LIMIT 150
    #endif

    #ifndef PLAYER_LIMIT
        #define PLAYER_LIMIT 10
    #endif

    #ifndef TEAM_LIMIT
        #define TEAM_LIMIT 150
    #endif

    #ifndef MAP_WIDTH
        #define MAP_WIDTH 10
    #endif


    #ifndef MAP_HEIGHT
        #define MAP_HEIGHT 10
    #endif


typedef struct s_map {
    size_t cells[MAP_HEIGHT][MAP_WIDTH];
    size_t player_count;
}   t_map;


typedef struct  s_player {
    size_t  x;
    size_t  y;
    size_t  team_id;
    size_t  index;
    pid_t   pid;
}   t_player;


typedef struct  s_teams {
    size_t player_ids[PLAYER_LIMIT];
    size_t count;
}   t_teams;



typedef struct  s_ipc {
    key_t       shm_key;
    key_t       sem_key;
    key_t       msg_key;
    int         shm_id;
    int         sem_id;
    int         msg_id;
    t_map       *map;
    t_teams     teams[TEAM_LIMIT];
    t_player    players[PLAYER_MAX_LIMIT];
}   t_ipc;


typedef struct  s_msg {
    long    mtype;
    char    mtext[128];
}   t_msg;

extern  bool g_has_running;

#endif