//
// Created by sifi on 18.04.22.
//


#include "../include/file_work.h"

// Just get file size
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        exit(-1);
    }
    return statbuf.st_size;
}

// Just allocate memory and add file attributes to the reallocated field
void add_file_info(files_t **to_add,
                   char *filename, char *file_path, char *hash) {
    (*to_add)->num++;
    (*to_add)->file_data = (file_data_t *) realloc(
            (*to_add)->file_data,
            (*to_add)->num * sizeof(file_data_t));
    strcpy((*to_add)->file_data[(*to_add)->num - 1].filename,
           filename);
    strcpy((*to_add)->file_data[(*to_add)->num - 1].path,
           file_path);
    strcpy((*to_add)->file_data[(*to_add)->num - 1].hash,
           hash);
}

// Function to collect unique files to the tree and duplicated files to the list
void collect_files(const char *current_dir,
                   node_t **unique_files,
                   list_t **duplicated_files,
                   flags_t flags) {

    char dir_name[PATH_MAX];
    sprintf(dir_name, "%s/", current_dir);
    dir_name[strlen(dir_name)] = '\0';
    DIR *dir = opendir(dir_name);

    if (!dir) {
        if (errno == EACCES) {
            printf("Cant open file: access error\n");
            closedir(dir);
            return;
        } else {
            perror("Opening file error");
            closedir(dir);
            exit(errno);
        }
    }

    struct dirent *file;

    while ((file = readdir(dir))) {
        char *filename = file->d_name;

        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;
        }

        if (!flags.all_files && filename[0] == '.') {
            continue;
        }

        char file_path[PATH_MAX];
        if (file->d_type == DT_DIR) {
            sprintf(file_path, "%s%s", dir_name, filename);
            collect_files(file_path, unique_files, duplicated_files, flags);
        }

        // If it's simple file
        int fd;
        unsigned long file_size;
        void *file_buffer;

        sprintf(file_path, "%s%s", dir_name, filename);
        fd = open(file_path, O_RDONLY);

        if (fd < 0) {
            close(fd);
            continue;
        }

        file_size = get_size_by_fd(fd);
        if (file_size > 100000000) {
            // If the file size is more than 1 GB - we cannot map it in virtual addresses
            if (flags.stats) {
                perror(("File size error in %s", filename));
            }
            close(fd);
            continue;
        }

        file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0);
        if (file_buffer == MAP_FAILED) {
            close(fd);
            munmap(file_buffer, file_size);
            if (flags.stats) {
                perror(("Mapping error in %s", filename));
            }
            continue;
        }

        unsigned char result[MD5_DIGEST_LENGTH];
        MD5((unsigned char *) (file_buffer), file_size, result);
        munmap(file_buffer, file_size);

        char *hash = (char *) malloc(MD5_DIGEST_LENGTH);
        hash = md5_to_string(result);

        printf("Name: %s; Hash: %s\n", filename, hash);

//        if((*unique_files)->file_data);
//        if ((*unique_files)->num == 0) {
//            add_file_info(duplicated_files, filename, file_path, hash);
//            close(fd);
//            continue;
//        }
//
//        bool is_in_files = false;
//
//        for (int i = 0; i < (*unique_files)->num; i++) {
//            if (strcmp((*unique_files)->file_data[i].hash, hash) == 0) {
//
//                // If we collect files with same extract, but different names
//                if (flags.name_flag) {
//                    // Skip, if names equals
//                    if (strcmp((*unique_files)->file_data->filename, filename) == 0) {
//                        add_file_info(duplicated_files, filename, file_path, hash);
//                        is_in_files = true;
//                        break;
//                    } else {
//                        add_file_info(unique_files, filename, file_path, hash);
//                        break;
//                    }
//                }
//
//                // If we should collect only unique files (don't have '-n' flag)
//                add_file_info(duplicated_files, filename, file_path, hash);
//
//                is_in_files = true;
//                break;
//            }
//        }
//
//        if (!is_in_files) {
//            add_file_info(unique_files, filename, file_path, hash);
//        }
//
//        close(fd);
//        free(hash);
//    }
//    closedir(dir);

    }
}


// Function to collect all files_t into array
//void collect_files(const char *current_dir,
//                   files_t **unique_files,
//                   files_t **duplicated_files,
//                   flags_t flags) {
//
//    char dir_name[PATH_MAX]; // +1 to '/'
//    sprintf(dir_name, "%s/", current_dir);
//    dir_name[strlen(dir_name)] = '\0';
//    DIR *dir = opendir(dir_name);
//
//    if (!dir) {
//        if (errno == EACCES) {
//            printf("Cant open file: access error\n");
//            closedir(dir);
//            return;
//        } else {
//            perror("Opening file error");
//            closedir(dir);
//            exit(errno);
//        }
//    }
//
//    struct dirent *file;
//
//    while ((file = readdir(dir))) {
//        char *filename = file->d_name;
//
//        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
//            continue;
//        }
//
//        if (!flags.all_files && filename[0] == '.') {
//            continue;
//        }
//
//        char file_path[PATH_MAX];
//        if (file->d_type == DT_DIR) {
//            sprintf(file_path, "%s%s", dir_name, filename);
//            collect_files(file_path, unique_files, duplicated_files, flags);
//        }
//
//        // If it's simple file
//        int fd;
//        unsigned long file_size;
//        void *file_buffer;
//
//        sprintf(file_path, "%s%s", dir_name, filename);
//        fd = open(file_path, O_RDONLY);
//
//        if (fd < 0) {
//            close(fd);
//            continue;
//        }
//
//        file_size = get_size_by_fd(fd);
//        if (file_size > 100000000) {
//            // If the file size is more than 1 GB - we cannot map it in virtual addresses
//            if (flags.stats) {
//                perror(("File size error in %s", filename));
//            }
//            close(fd);
//            continue;
//        }
//
//        file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0);
//        if (file_buffer == MAP_FAILED) {
//            close(fd);
//            munmap(file_buffer, file_size);
//            if (flags.stats) {
//                perror(("Mapping error in %s", filename));
//            }
//            continue;
//        }
//
//        unsigned char result[MD5_DIGEST_LENGTH];
//        MD5((unsigned char *) (file_buffer), file_size, result);
//        munmap(file_buffer, file_size);
//
//        char *hash = (char *) malloc(MD5_DIGEST_LENGTH);
//        hash = md5_to_string(result);
//
//        printf("Name: %s; Hash: %s\n", filename, hash);
//
//        if ((*unique_files)->num == 0) {
//            add_file_info(duplicated_files, filename, file_path, hash);
//            close(fd);
//            continue;
//        }
//
//        bool is_in_files = false;
//
//        for (int i = 0; i < (*unique_files)->num; i++) {
//            if (strcmp((*unique_files)->file_data[i].hash, hash) == 0) {
//
//                // If we collect files with same extract, but different names
//                if (flags.name_flag) {
//                    // Skip, if names equals
//                    if (strcmp((*unique_files)->file_data->filename, filename) == 0) {
//                        add_file_info(duplicated_files, filename, file_path, hash);
//                        is_in_files = true;
//                        break;
//                    } else {
//                        add_file_info(unique_files, filename, file_path, hash);
//                        break;
//                    }
//                }
//
//                // If we should collect only unique files (don't have '-n' flag)
//                add_file_info(duplicated_files, filename, file_path, hash);
//
//                is_in_files = true;
//                break;
//            }
//        }
//
//        if (!is_in_files) {
//            add_file_info(unique_files, filename, file_path, hash);
//        }
//
//        close(fd);
//        free(hash);
//    }
//    closedir(dir);
//}