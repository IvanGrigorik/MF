//
// Created by sifi on 17.04.22.
//

#ifndef UTILITY_C_STRUCTURES_H
#define UTILITY_C_STRUCTURES_H

#include <stdbool.h>

typedef struct file_data {
    char file_name[256];
    char file_hash[33];
    char file_path[256];

//    file_data(std::string fileName, std::string fileHash) {
//        file_name = std::move(fileName);
//        file_hash = std::move(fileHash);
//    }
} file_data_t;

typedef struct file_struct{
    file_data_t *file_data;
    int num;
}files;

/*typedef struct file_to_delete {
    char file_name[256];
    char file_path[256];

//    file_to_delete(std::string fileName, std::string filePath) {
//        file_name = std::move(fileName);
//        file_path = std::move(filePath);
//    }
} file_to_delete_t;*/

typedef struct flags {
    bool stats;
    bool name_flag;
    bool delete_flag;
    bool all_files;
    bool test_flag;
} flags_t;

#endif //UTILITY_C_STRUCTURES_H
