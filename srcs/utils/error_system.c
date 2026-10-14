#include <errno.h>
#include <unistd.h>
#include <string.h>

void    print_error_system(const char *src) {
    const char  *msg;

    if (!src) return;

    msg = strerror(errno);
    write(STDERR_FILENO, src, strlen(src));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
}
