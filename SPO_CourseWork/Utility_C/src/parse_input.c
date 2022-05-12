//
// Created by sifi on 17.04.22.
//

#include "../headers/parse_input.h"

char *get_dir(int argc, char *argv[]) {
    char *dir_to_find = (argc == 1 || argv[1][0] == '-') ? "." : argv[1];

    if (argc == 1 || argv[1][0] == '-') {
        dir_to_find = argv[2];
    } else {
        dir_to_find = argv[1];
    }

    for (int i = (int) strlen(dir_to_find) - 1; i > 0; i--) {
        if (dir_to_find[i] == '/') {
            dir_to_find[i] = '\0';
        } else {
            break;
        }
    }
    if (strcmp(dir_to_find, "~") == 0 || strcmp(dir_to_find, "/") == 0) {
        strcpy(dir_to_find, "/home/");
        strcat(dir_to_find, getlogin());
    }

    return dir_to_find;
}

void init_flags(flags_t *flags) {

    flags->all_files = false;
    flags->test_flag = false;
    flags->delete_flag = false;
    flags->name_flag = false;
    flags->stats = false;
    flags->help_flag = false;
    flags->recursive_flag = false;
}

flags_t parse_flags(int argc, char *argv[]) {

    flags_t flags;
    init_flags(&flags);
    int opt;

    while ((opt = getopt(argc, argv, "adhnstr")) != -1) {
        switch (opt) {
            case 'd':
                flags.delete_flag = true;
                break;

            case 'n':
                flags.name_flag = true;
                break;

            case 's':
                flags.stats = true;
                break;

            case 'a':
                flags.all_files = true;
                break;

            case 't':
                flags.test_flag = true;
                break;

            case 'h':
                flags.help_flag = true;
                break;

            case 'r':
                flags.recursive_flag = true;
                break;

            default:
                perror("Flag parse error");
        }
    }

    return flags;
}