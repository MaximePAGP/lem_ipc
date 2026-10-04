#include "../inc/ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


static  void    display_usage(void) {
    const char *usage_msg = "Usage: ./lem_ipc <team_id>\n";
    const char *team_id_description = "<team_id> must be an integer between 1 and 150\n";

    write(STDERR_FILENO, usage_msg, strlen(usage_msg));
    write(STDERR_FILENO, team_id_description, strlen(team_id_description));
}


static bool is_only_digits(const char *str) {
    if (!*str || strlen(str) > 9)
        return (false);
    for (size_t i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return (false);
    }
    return (true);
}

int handle_args(int argc, char **argv) {
    if (argc != 2) {
        display_usage();
        exit(EXIT_FAILURE);
    }

    int team_id = is_only_digits(argv[1]) ? atoi(argv[1]) : -1;

    if (team_id < 1 || team_id > TEAM_LIMIT) {
        const char *error_msg = "Error: Invalid team_id, \n";
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        display_usage();
        exit(EXIT_FAILURE);
    }
    return (team_id);
}
