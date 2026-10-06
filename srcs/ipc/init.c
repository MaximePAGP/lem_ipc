#include "../inc/ipc.h"
#include <errno.h>
#include <string.h>

void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);


static  void    catch_error(t_ipc *ipc, const char *err_msg, bool is_creator) {
    write(STDERR_FILENO, err_msg, strlen(err_msg));

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
        catch_error(ipc, "shmget\n", true);

    ipc->msg_id = msgget(ipc->msg_key, IPC_CREAT | IPC_EXCL | 0666);
    if (ipc->msg_id == -1)
        catch_error(ipc, "msgget\n", true);

    ipc->map = shmat(ipc->shm_id, NULL, 0);
    if (ipc->map == (void *)-1) {
        ipc->map = NULL;
        catch_error(ipc, "shmat\n", true);
    }

    ipc->map->player_count = 1;

    if (semctl(ipc->sem_id, 0, SETVAL, 1) == -1)
        catch_error(ipc, "semctl SETVAL\n", true);
}

static  void    join_ipc(t_ipc *ipc) {
    ipc->sem_id = semget(ipc->sem_key, 1, 0666);
    if (ipc->sem_id == -1)
        catch_error(ipc, "semget\n", false);

    sem_lock(ipc->sem_id);

    ipc->shm_id = shmget(ipc->shm_key, sizeof(t_map), 0666);
    if (ipc->shm_id == -1)
        catch_error(ipc, "shmget\n", false);

    ipc->msg_id = msgget(ipc->msg_key, 0666);
    if (ipc->msg_id == -1)
        catch_error(ipc, "msgget\n", false);

    ipc->map = shmat(ipc->shm_id, NULL, 0);
    if (ipc->map == (void *)-1) {
        ipc->map = NULL;
        catch_error(ipc, "shmat\n", false);
    }

    ipc->map->player_count++;
    sem_unlock(ipc->sem_id);
}

void    init_ipc(t_ipc *ipc) {
    ipc->shm_key = ftok("/tmp", 'S');
    ipc->sem_key = ftok("/tmp", 'M');
    ipc->msg_key = ftok("/tmp", 'Q');

    if (ipc->shm_key == -1 || ipc->sem_key == -1 || ipc->msg_key == -1)
        catch_error(ipc, "ftok\n", false);

    ipc->sem_id = semget(ipc->sem_key, 1, IPC_CREAT | IPC_EXCL | 0666);

    if (ipc->sem_id == -1 && errno != EEXIST)
        catch_error(ipc, "semget\n", false);

    if (ipc->sem_id != -1)
        create_ipc(ipc);
    else if (errno == EEXIST)
        join_ipc(ipc);
}
