

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


static  void    display_usage(void) {
    const char *usage_msg = "Usage: ./lem_ipc <team_id>\n";
    const char *team_id_description = "<team_id> must be an integer between 0 and 25\n";

    write(STDERR_FILENO, usage_msg, strlen(usage_msg));
    write(STDERR_FILENO, team_id_description, strlen(team_id_description));
}

static bool is_valid_team_id(const char *team_id) {
    int id = atoi(team_id);

    return (id >= 0 && id <= 25);
}

void    handle_args(int argc, char **argv) {
    if (argc != 2) {
        display_usage();
        exit(EXIT_FAILURE);
    }
    if (!is_valid_team_id(argv[1])) {
        const char *error_msg = "Error: Invalid team_id, \n";
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        display_usage();
        exit(EXIT_FAILURE);
    }
}
