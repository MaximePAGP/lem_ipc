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


    #ifndef MAP_WIDTH
        #define MAP_WIDTH 9
    #endif


    #ifndef MAP_HEIGHT
        #define MAP_HEIGHT 9
    #endif


typedef struct s_map {
    size_t cells[MAP_HEIGHT][MAP_WIDTH];
}   t_map;



typedef enum e_teams {
    TEAM_RED,
    TEAM_BLUE,
    TEAM_GREEN,
    TEAM_YELLOW
}   e_teams;

typedef struct  s_player {
    size_t  x;
    size_t  y;
    e_teams team;
    pid_t   pid;
}   t_player;


typedef struct  s_ipc {
    key_t   shm_key;
    key_t   sem_key;
    key_t   msg_key;
    int     shm_id;
    int     sem_id;
    int     msg_id;
    t_map   *map;
}   t_ipc;


typedef struct  s_msg {
    long    mtype;
    char    mtext[128];
}   t_msg;

extern  bool g_has_running;

#endif