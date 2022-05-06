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
#include "structures.h"


void find_duplicated(const char *current_dir,
                     list_t **unique_files,
                     list_t **duplicated_files,
                     list_t **error_files,
                     flags_t flags);

void output_statistic(list_t *unique_files,
                      list_t *duplicated_files,
                      list_t *error_files,
                      flags_t flags);

void check_duplicated(char *filename, char *file_path, char *hash,
                      list_t **unique_files,
                      list_t **duplicated_files,
                      flags_t flags);

void delete_files(list_t **duplicated_files);

int get_int(int min_size, int max_size);

#endif //UTILITY_C_FILE_WORK_H
