//
// Created by sifi on 18.04.22.
//


#include "../include/file_work.h"

// Get file size by it descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        exit(-1);
    }
    return statbuf.st_size;
}

// Just allocate memory and add file attributes
void add_file_info(list_t **to_add,
                   char *filename, char *file_path, char *hash) {

    list_t *temp;
    temp = (list_t *) malloc(sizeof(list_t));
    temp->next = NULL;
    strcpy(temp->file_data.filename, filename);
    strcpy(temp->file_data.path, file_path);
    strcpy(temp->file_data.hash, hash);

    if ((*to_add) == NULL) {
        *to_add = temp;
        (*to_add)->tail = (*to_add);
    } else {
        (*to_add)->tail->next = temp;
        (*to_add)->tail = (*to_add)->tail->next;
    }
}

// Function to collect unique files to the tree and duplicated files to the list
void collect_files(const char *current_dir,
                   list_t **unique_files,
                   list_t **duplicated_files,
                   flags_t flags) {
    char dir_name[PATH_MAX];
    DIR *dir = opendir(strcat(strcpy(dir_name, current_dir), "/"));

    if (!dir) {
        if (errno == EACCES) {
            puts("Cant open file: access error");
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
        sprintf(file_path, "%s%s", dir_name, filename);
        if (file->d_type == DT_DIR) {
            collect_files(file_path, unique_files, duplicated_files, flags);
            continue;
        }

        // If it's simple file
        int fd;
        size_t file_size;
        void *file_buffer;

        if ((fd = open(file_path, O_RDONLY)) < 0) {
            close(fd);
            continue;
        }

        file_size = get_size_by_fd(fd);
        if (file_size > 1000000) {
            // If the file size is more than 1 GB - we cannot map it in virtual addresses
            if (flags.stats) {
                perror(("File size error in %s", filename));
            }
            close(fd);
            continue;
        }

        file_buffer = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
        close(fd);
        if (file_buffer == MAP_FAILED) {
            munmap(file_buffer, file_size);
            if (flags.stats) {
                perror(("Mapping error in %s", filename));
            }
            continue;
        }

        unsigned char result[MD5_DIGEST_LENGTH];
        MD5((unsigned char *) (file_buffer), file_size, result);
        munmap(file_buffer, file_size);

        char *hash = md5_to_string(result);

        if ((*unique_files) == NULL) {
            add_file_info(unique_files, filename, file_path, hash);
            continue;
        }

        bool is_in_files = false;
        list_t *ptr = *unique_files;
        while (true) {
            // If file in the list and current file same
            if (strcmp(ptr->file_data.hash, hash) == 0) {
                // If we collect files with same extract, but different names
                if (flags.name_flag) {
                    // Skip, if name equals
                    if (strcmp(ptr->file_data.filename, filename) == 0) {
                        add_file_info(duplicated_files, filename, file_path, hash);
                        is_in_files = true;
                        break;
                    } else {
                        if (ptr->next != NULL) {
                            ptr = ptr->next;
                        } else {
                            is_in_files = false;
                            break;
                        }
                        continue;
                    }
                }
                // If we should collect only unique files (don't have '-n' flag)
                add_file_info(duplicated_files, filename, file_path, hash);

                is_in_files = true;
                break;
            }
            if (ptr->next != NULL) {
                ptr = ptr->next;
            } else {
                is_in_files = false;
                break;
            }
        }

        if (!is_in_files) {
            add_file_info(unique_files, filename, file_path, hash);
        }
        free(hash);
    }
    closedir(dir);
}


void output_statistic(list_t *unique_files,
                      list_t *duplicated_files,
                      flags_t flags) {

    int total_unique = 0, total_duplicate = 0;
    printf("Total unique: %d \nTotal duplicated: %d\n", total_unique, total_duplicate);

    if (unique_files == NULL && duplicated_files == NULL) {
        printf(RED "Empty directory" WHITE "\n");
        return;
    }

    if (unique_files == NULL) {
        printf(RED "No unique files" WHITE "\n");
    } else {
        list_t *ptr = unique_files;
        printf(GREEN "\n");
        while (ptr->next != NULL) {
            ++total_unique;
            printf("File name: %s\n", ptr->file_data.filename);
            printf("File directory: %s\n", ptr->file_data.path);
            ptr = ptr->next;
        }
    }


    if (duplicated_files == NULL) {
        printf(RED "No duplicated files" WHITE "\n");
    } else {
        list_t *ptr = duplicated_files;
        printf(YELLOW "\n");
        while (ptr->next != NULL) {
            ++total_duplicate;
            printf("File name: %s\n", ptr->file_data.filename);
            printf("File directory: %s\n", ptr->file_data.path);
            ptr = ptr->next;
        }
    }

    printf(CYAN "Total unique: %d\n", total_unique);
    printf("Total duplicated: %d", total_duplicate);
    printf( WHITE);
}