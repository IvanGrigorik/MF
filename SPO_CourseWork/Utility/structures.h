//
// Created by sifi on 11.04.2022.
//

#ifndef SPO_COURSEWORK_STRUCTURES_H
#define SPO_COURSEWORK_STRUCTURES_H

typedef struct file_data {
    std::string file_name{};
    std::string file_hash{};

    file_data(std::string fileName, std::string fileHash) {
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
    bool reverse_flag{};
} flags_t;

#endif //SPO_COURSEWORK_STRUCTURES_H
