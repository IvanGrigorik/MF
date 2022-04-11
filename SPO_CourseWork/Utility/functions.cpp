//
// Created by sifi on 28.03.2022.
//


#include "functions.h"

int num = 0;
unsigned long all_files_size = 0;

std::string get_dir(int argc, char *argv[]) {
    std::string dir_to_find = (argc == 1 || argv[1][0] == '-') ? "." : argv[1];

    if (argc == 1 || argv[1][0] == '-') {
        dir_to_find = argv[2];
    } else {
        dir_to_find = argv[1];
    }
    if (dir_to_find.back() == '/') {
        dir_to_find.pop_back();
    }
    return dir_to_find;
}

std::string md5_to_string(unsigned char *md) {

    std::stringstream s_stream{};

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        s_stream << std::hex << static_cast<int>(md[i]);
    }

    return s_stream.str();
}


/* Function to collect files hash to files_vector
 * if the flag is 'n' (names) - we collect files with the same hash, but different names.
 * if the flag is 'd' (default) - we collect files with unique hash (don't look at names).
*/
void collect_files(const std::string &current_dir,
                   std::vector<file_data_t> &unique_files,
                   std::vector<file_to_delete_t> &duplicated_files,
                   flags_t flags) {
    unsigned char result[MD5_DIGEST_LENGTH];

    DIR *dir = opendir((current_dir + "/").c_str());

    if (!dir) {
        if (errno == EACCES) {
            std::cout << "Cant open file: access error" << std::endl;
            closedir(dir);
            return;
        } else {
            std::cerr << strerror(errno) << std::endl;
            closedir(dir);
            exit(errno);
        }
    }

    dirent *file;
    while ((file = readdir(dir))) {
        std::string filename = file->d_name;
        if ((filename == "." || filename == "..")) {
            continue;
        }

        if (!flags.all_files && filename[0] == '.') {
            continue;
        }

        if (file->d_type == DT_DIR) {
            collect_files(current_dir + '/' + filename, unique_files, duplicated_files, flags);
            continue;
        }
        // If it not directory:

        int file_descript;
        unsigned long file_size;
        void *file_buffer;

        file_descript = open((current_dir + '/' + filename).c_str(), O_RDONLY);

        if (file_descript < 0) {
            continue;
        }

        file_size = get_size_by_fd(file_descript);
        if (file_size > 1'000'000) {
            if (flags.stats) {
                perror(("File size error in " + filename).c_str());
            }
            continue;
        }

        // Функция возвращает адрес начала участка отображаемой памяти или MAP_FAILED в случае неудачи.
        file_buffer = mmap(nullptr, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
        if (file_buffer == MAP_FAILED) {
            close(file_descript);
            munmap(file_buffer, file_size);
            if (flags.stats) {
                perror(("Mapping error in " + filename).c_str());
            }
            continue;
        }
        MD5(reinterpret_cast <unsigned char *>(file_buffer), file_size, result);
        munmap(file_buffer, file_size);

        std::string hash = md5_to_string(result);

        if (flags.stats) {
            all_files_size += file_size;
            std::cout << "All files size: " << all_files_size << std::endl;
            std::cout << ++num << std::endl;
            std::cout << "File descript: " << file_descript << std::endl;
        }

        if (unique_files.empty()) {
            unique_files.emplace_back(filename, hash);
            continue;
        }

        bool is_in_files{};
        for (int i = 0; i < unique_files.size(); i++) {
            if (unique_files[i].file_hash == hash) {

                // If we collect files with same hash, but different names
                if (flags.name_flag) {
                    // If names equal - skip
                    if (unique_files[i].file_name == filename) {
                        duplicated_files.emplace_back(file_to_delete_t(filename, (current_dir + '/' + filename)));
                        is_in_files = true;
                        break;
                    } else {
                        unique_files.emplace_back(filename, hash);
                        break;
                    }
                }
                // And if we should collect only unique hash
                duplicated_files.emplace_back(file_to_delete_t(filename, (current_dir + '/' + filename)));
                is_in_files = true;
            }
        }

        if (!is_in_files) {
            unique_files.emplace_back(filename, hash);
        }

        close(file_descript);
    }
    closedir(dir);
}

flags_t parse_flags(int argc, char *argv[]) {
    flags_t flags;

    int opt;

    while ((opt = getopt(argc, argv, "adnst")) != -1) {

        switch (opt) {
            case 'd':
                flags.delete_flag = true;
                break;

            case 'n':
                flags.name_flag = true;
                break;

            case 's':
                flags.stats = true;
                break;

            case 'a':
                flags.all_files = true;
                break;

            case 't':
                flags.test_flag = true;
                break;

            default:
                throw (std::runtime_error("Parse flags error"));
        }
    }

    return flags;
}

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf{};
    if (fstat(fd, &statbuf) < 0) {
        exit(-1);
    }
    return statbuf.st_size;
}


void files_output(const std::vector<file_data_t> &unique_files,
                  const std::vector<file_to_delete_t> &duplicated_files,
                  flags_t flags) {

    if (unique_files.empty() && duplicated_files.empty()) {
        std::cout << RED << "Empty directory" << WHITE << std::endl;
        return;
    }

    if (unique_files.empty()) {
        std::cout << RED << "No unique files!" << WHITE << std::endl;
    } else {
        std::cout << GREEN << "Unique files: " << std::endl;
        for (auto &i: unique_files) {
            std::cout << "File name: " << i.file_name << std::endl
                      << "File hash: " << i.file_hash << std::endl << std::endl;
        }
        std::cout << WHITE;
    }

    if (duplicated_files.empty()) {
        std::cout << RED << "No duplicated files!" << WHITE << std::endl;
    } else {
        std::cout << YELLOW << "Duplicated files: " << std::endl;
        for (auto &i: duplicated_files) {
            std::cout << "File name: " << i.file_name << std::endl
                      << "File directory: " << i.file_path << std::endl;
        }
        std::cout << WHITE;
    }

    if (flags.stats) {
        std::cout << CYAN << std::endl
                  << "Total unique flags: " << unique_files.size() << std::endl
                  << "Total duplicated files: " << duplicated_files.size() << std::endl << WHITE;
    }
}

void delete_files(std::vector<file_to_delete_t> &duplicated_files, flags_t flags) {
    std::cout << RED << "Do you really want to delete all duplicated files (Y/N)" << std::endl
              << "(it can be fatal to delete from root or home directory)" << std::endl << WHITE
              << "> ";

    int choice = getchar();

    switch (choice) {
        case 'Y':
            for (auto &i: duplicated_files)
                std::remove(i.file_path.c_str());
            break;

        case 'N':
            if (!flags.stats) {
                std::cout << CYAN << "Total duplicated files: " << duplicated_files.size() << std::endl << WHITE;
            }
            break;

        default:
            break;
    }
}