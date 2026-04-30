#include "ipc.h"

#include <stdlib.h>

bool    g_has_running = true;


void    init_signals(void);
void    handle_args(int argc, char **argv);

int main(int argc, char **argv) {
    init_signals();
    handle_args(argc, argv);
    while (g_has_running) {

    }
    return (EXIT_SUCCESS);
}
