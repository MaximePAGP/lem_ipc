#include "ipc.h"
#include "utils.h"

#include <time.h>

void    init_random(void) {
    srand(getpid() ^ time(NULL));
}

size_t  random_range(size_t max) {
    if (max == 0)
        return (0);
    return ((size_t)rand() % max);
}

void    random_position(size_t *x, size_t *y) {
    *x = random_range(MAP_WIDTH);
    *y = random_range(MAP_HEIGHT);
}
