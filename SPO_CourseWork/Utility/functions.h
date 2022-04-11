//
// Created by sifi on 28.03.2022.
//

#ifndef SPO_COURSEWORK_FUNCTIONS_H
#define SPO_COURSEWORK_FUNCTIONS_H

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <dirent.h>
#include <cstring>

#include <openssl/md5.h>
#include <map>
#include <utility>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <csignal>

#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

typedef struct file_data_t {
    std::string file_name{};
    std::string file_hash{};

    file_data_t(std::string fileName, std::string fileHash) {
        file_name = std::move(fileName);
        file_hash = std::move(fileHash);
    }
} file_data_t;

typedef struct file_to_delete {
    std::string file_name{};
    std::string file_path{};

    file_to_delete(std::string fileName, std::string filePath) {
        file_name = std::move(fileName);
        file_path = std::move(filePath);
    }
} file_to_delete_t;

typedef struct flags {
    bool stats{};
    bool name_flag{};
    bool delete_flag{};
    bool all_files{};
    bool test_flag{};
    bool reverse_flag;
} flags_t;

void collect_files(const std::string &current_dir,
                   std::vector<file_data_t> &unique_files,
                   std::vector<file_to_delete_t> &duplicated_files,
                   flags_t flags);

unsigned long get_size_by_fd(int fd);

flags_t parse_flags(int argc, char *argv[]);

void files_output(const std::vector<file_data_t> &unique_files,
                  const std::vector<file_to_delete_t> &duplicated_files,
                  flags_t flags);

void delete_files(std::vector<file_to_delete_t> &duplicated_files, flags_t flags);

std::string get_dir(int argc, char *argv[]);

#endif //SPO_COURSEWORK_FUNCTIONS_H