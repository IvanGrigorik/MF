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

// Functions to collect all duplicated and unique files
void find_duplicated(const char *current_dir,
                     list_t **unique_files,
                     list_t **duplicated_files,
                     list_t **error_files,
                     flags_t flags);

void check_duplicated(char *filename, char *file_path, char *hash, int file_type,
                      list_t **unique_files,
                      list_t **duplicated_files,
                      flags_t flags);

// Simple statistic output
void output_statistic(list_t *unique_files,
                      list_t *duplicated_files,
                      list_t *error_files,
                      flags_t flags);

void check_by_name(char *filename, char *file_path, int file_type,
                   list_t **unique_files,
                   list_t **duplicated_files);

// Functions to delete process
void delete_file(list_t **duplicated_files, int choice);

void delete_all_files(list_t *duplicated_files);

void delete_process(list_t **duplicated_files);

#endif //UTILITY_C_FILE_WORK_H
