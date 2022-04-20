//
// Created by sifi on 18.04.22.
//

#ifndef UTILITY_C_FILE_WORK_H
#define UTILITY_C_FILE_WORK_H

#include <dirent.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "hashing.h"
#include "../include/structures.h"

void collect_files(const char *current_dir,
                   files_t **unique_files,
                   files_t **duplicated_files,
                   flags_t flags);

#endif //UTILITY_C_FILE_WORK_H
