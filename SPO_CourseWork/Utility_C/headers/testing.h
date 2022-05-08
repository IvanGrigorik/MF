//
// Created by sifi on 5/5/22.
//

#ifndef C_TESTING_H
#define C_TESTING_H

#include "../headers/structures.h"

// To wrap all functions
void full_test();

// To generate 9 random files with pseudorandom content
void generate_files();

// Change flags and call functions work with files info
void test_duplicated();

// Clear all lists, that are passed to it
void clear_lists(list_t **unique_files, list_t **duplicated_files, list_t **error_files);

#endif //C_TESTING_H
