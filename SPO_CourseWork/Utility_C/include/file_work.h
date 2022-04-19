//
// Created by sifi on 18.04.22.
//

#ifndef UTILITY_C_FILE_WORK_H
#define UTILITY_C_FILE_WORK_H

#include <dirent.h>
#include <stdio.h>
#include <malloc.h>

#include "hashing.h"
#include "../include/structures.h"

void collect_files(const char *current_dir,
                   files **unique_files,
                   files **duplicated_files,
                   flags_t flags);

#endif //UTILITY_C_FILE_WORK_H
