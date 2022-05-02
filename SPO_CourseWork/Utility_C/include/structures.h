//
// Created by sifi on 17.04.22.
//

#ifndef UTILITY_C_STRUCTURES_H
#define UTILITY_C_STRUCTURES_H

#include <stdbool.h>

#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

typedef struct file_data {
    char filename[256];
    char hash[33];
    char path[4096];
} file_data_t;

typedef struct file_struct {
    file_data_t *file_data;
    int num;
} files_t;

typedef struct node {
    file_data_t file_data;
    struct node *left;
    struct node *right;
} node_t;

typedef struct list {
    file_data_t file_data;
    struct list *next;
} list_t;

typedef struct flags {
    bool stats;
    bool name_flag;
    bool delete_flag;
    bool all_files;
    bool test_flag;
} flags_t;

#endif //UTILITY_C_STRUCTURES_H
