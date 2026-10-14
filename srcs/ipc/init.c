#include "../inc/ipc.h"
#include "../inc/utils.h"
#include <errno.h>
#include <string.h>


static  void    catch_error(t_ipc *ipc, const char *src, bool is_creator) {
    print_error_system(src);

    if (ipc->map) {
        shmdt(ipc->map);
        ipc->map = NULL;
    }

    if (is_creator) {
        if (ipc->shm_id != -1)
            shmctl(ipc->shm_id, IPC_RMID, NULL);
        if (ipc->msg_id != -1)
            msgctl(ipc->msg_id, IPC_RMID, NULL);
        if (ipc->sem_id != -1)
            semctl(ipc->sem_id, 0, IPC_RMID);
    } else if (ipc->sem_id != -1)
        sem_unlock(ipc->sem_id);

    exit(EXIT_FAILURE);
}


static  void    create_ipc(t_ipc *ipc) {
    ipc->shm_id = shmget(ipc->shm_key, sizeof(t_map), IPC_CREAT | IPC_EXCL | 0666);
    if (ipc->shm_id == -1)
        catch_error(ipc, "init.c: shmget", true);

    ipc->msg_id = msgget(ipc->msg_key, IPC_CREAT | IPC_EXCL | 0666);
    if (ipc->msg_id == -1)
        catch_error(ipc, "init.c: msgget", true);

    ipc->map = shmat(ipc->shm_id, NULL, 0);
    if (ipc->map == (void *)-1) {
        ipc->map = NULL;
        catch_error(ipc, "init.c: shmat", true);
    }

    ipc->map->player_count = 1;

    if (semctl(ipc->sem_id, 0, SETVAL, 1) == -1)
        catch_error(ipc, "init.c: semctl SETVAL", true);
}

static  void    join_ipc(t_ipc *ipc) {
    ipc->sem_id = semget(ipc->sem_key, 1, 0666);
    if (ipc->sem_id == -1)
        catch_error(ipc, "init.c: semget", false);

    sem_lock(ipc->sem_id);

    ipc->shm_id = shmget(ipc->shm_key, sizeof(t_map), 0666);
    if (ipc->shm_id == -1)
        catch_error(ipc, "init.c: shmget", false);

    ipc->msg_id = msgget(ipc->msg_key, 0666);
    if (ipc->msg_id == -1)
        catch_error(ipc, "init.c: msgget", false);

    ipc->map = shmat(ipc->shm_id, NULL, 0);
    if (ipc->map == (void *)-1) {
        ipc->map = NULL;
        catch_error(ipc, "init.c: shmat", false);
    }

    ipc->map->player_count++;
    sem_unlock(ipc->sem_id);
}

void    init_ipc(t_ipc *ipc) {
    ipc->shm_key = ftok("/tmp", 'S');
    ipc->sem_key = ftok("/tmp", 'M');
    ipc->msg_key = ftok("/tmp", 'Q');

    if (ipc->shm_key == -1 || ipc->sem_key == -1 || ipc->msg_key == -1)
        catch_error(ipc, "init.c: ftok", false);

    ipc->sem_id = semget(ipc->sem_key, 1, IPC_CREAT | IPC_EXCL | 0666);

    if (ipc->sem_id == -1 && errno != EEXIST)
        catch_error(ipc, "init.c: semget", false);

    if (ipc->sem_id != -1)
        create_ipc(ipc);
    else if (errno == EEXIST)
        join_ipc(ipc);
}
