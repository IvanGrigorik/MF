//
// Created by sifi on 18.04.22.
//

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_work.h"


// Function to collect all files into array
void collect_files(const char *current_dir,
                   files **unique_files,
                   files **duplicated_files,
                   flags_t flags) {

    char dir_name[256]; // +1 to '/'
    sprintf(dir_name, "%s/", current_dir);
    dir_name[strlen(dir_name)] = '\0';
    DIR *dir = opendir(dir_name);

    if (!dir) {
        if (errno == EACCES) {
            printf("Cant open file: access error\n");
            closedir(dir);
            return;
        } else {
            perror("Opening file error");
            closedir(dir);
            exit(errno);
        }
    }

    struct dirent *file;

    while ((file = readdir(dir))) {
        char *filename = file->d_name;

        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;
        }

        if (!flags.all_files && filename[0] == '.') {
            continue;
        }

        if (file->d_type == DT_DIR) {
            char to_collect[256];
            sprintf(to_collect, "%s%s", dir_name, filename);
            collect_files(to_collect, unique_files, duplicated_files, flags);
        }

    }


}