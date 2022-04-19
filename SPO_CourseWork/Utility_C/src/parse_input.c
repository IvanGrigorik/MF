//
// Created by sifi on 17.04.22.
//

#include "../include/parse_input.h"

char *get_dir(int argc, char *argv[]) {
    char *dir_to_find = (argc == 1 || argv[1][0] == '-') ? "." : argv[1];

    if (argc == 1 || argv[1][0] == '-') {
        dir_to_find = argv[2];
    } else {
        dir_to_find = argv[1];
    }

    if(dir_to_find[strlen(dir_to_find) - 1] == '/'){
        dir_to_find[strlen(dir_to_find) - 1] = '\0';
    }

    return dir_to_find;
}


flags_t parse_flags(int argc, char *argv[]) {
    flags_t flags;

    int opt;

    while ((opt = getopt(argc, argv, "adnst")) != -1) {

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

            default:
                perror("Flag parse error");
        }
    }

    return flags;
}