#include "ipc.h"

#include <signal.h>
#include <stddef.h>

static  void    handle_signal(int signum) {
    (void)signum;
    g_has_running = false;
}


void	init_signals(void) {
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

