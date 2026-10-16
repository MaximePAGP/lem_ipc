#ifndef UTILS_H
#define UTILS_H

void    print_error_system(const char *src);
void    print_msg(int fd, const char *msg);
void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);

#endif
