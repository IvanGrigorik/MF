//
// Created by sifi on 11.04.2022.
//

#ifndef SPO_COURSEWORK_TEST_LAUNCH_H
#define SPO_COURSEWORK_TEST_LAUNCH_H

#include "functions.h"

void generate_files();

void show_search(std::vector<file_data_t> &unique_files,
                 std::vector<file_to_delete_t> &duplicated_files);

void output_file_content();


#endif //SPO_COURSEWORK_TEST_LAUNCH_H
