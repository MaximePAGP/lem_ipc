#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void    print_error_system(const char *src);
void    print_msg(int fd, const char *msg);
void    sem_lock(int sem_id);
void    sem_unlock(int sem_id);

void    init_random(void);
size_t  random_range(size_t max);
void    random_position(size_t *x, size_t *y);

#endif
