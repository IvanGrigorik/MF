//
// Created by sifi on 18.04.22.
//

#include "../headers/file_work.h"

#define GB 1000000

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
void find_duplicated(const char *current_dir,
                     list_t **unique_files,
                     list_t **duplicated_files,
                     list_t **error_files,
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
            find_duplicated(file_path, unique_files, duplicated_files, error_files, flags);
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
        if (file_size > GB) {
            // If the file size is more than 1 GB - we cannot map it in virtual addresses
            if (flags.stats) {
                add_file_info(error_files, filename, file_path, "Size error");
                perror("File size error in ");
            }
            close(fd);
            continue;
        }

        file_buffer = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
        close(fd);
        if (file_buffer == MAP_FAILED) {
            munmap(file_buffer, file_size);
            if (flags.stats) {
                add_file_info(error_files, filename, file_path, "Mmap error");
                perror("Mapping error");

            }
            continue;
        }

        unsigned char result[MD5_DIGEST_LENGTH];
        MD5((unsigned char *) (file_buffer), file_size, result);
        munmap(file_buffer, file_size);

        char *hash = md5_to_string(result);

        check_duplicated(filename, file_path, hash, unique_files, duplicated_files, flags);

        free(hash);
    }
    closedir(dir);
}

void check_duplicated(char *filename, char *file_path, char *hash,
                      list_t **unique_files,
                      list_t **duplicated_files,
                      flags_t flags) {

    if ((*unique_files) == NULL) {
        add_file_info(unique_files, filename, file_path, hash);
        return;
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
}

void output_statistic(list_t *unique_files,
                      list_t *duplicated_files,
                      list_t *error_files,
                      flags_t flags) {

    if (unique_files == NULL && duplicated_files == NULL) {
        printf(RED "Empty directory" WHITE "\n");
        return;
    }

    int total_unique = 0;
    if (unique_files == NULL) {
        printf(RED "No unique files" WHITE "\n");
    } else {
        list_t *ptr = unique_files;
        printf(GREEN "\n");
        while (ptr != NULL) {
            ++total_unique;
            printf("File path: %s\n", ptr->file_data.path);
            ptr = ptr->next;
        }
    }

    int total_duplicate = 0;
    if (duplicated_files == NULL) {
        printf(RED "No duplicated files" WHITE "\n");
    } else {
        list_t *ptr = duplicated_files;
        printf(YELLOW "\n");
        while (ptr != NULL) {
            ++total_duplicate;
            printf("File path: %s\n", ptr->file_data.path);
            ptr = ptr->next;
        }
    }

    if (flags.stats) {
        if (error_files == NULL) {
            printf(RED "No files with errors" WHITE "\n");
        } else {
            int total_error = 0;
            list_t *ptr = error_files;
            printf(RED "\n");
            while (ptr != NULL) {
                ++total_error;
                printf("File path: %s\n", ptr->file_data.path);
                printf("File error: %s\n", ptr->file_data.hash);
                ptr = ptr->next;
            }
            printf(RED "Total files with error: %d\n", total_error);
        }
    }

    printf(CYAN "Total unique: %d\n", total_unique);
    printf("Total duplicated: %d", total_duplicate);
    printf(WHITE);
}

void delete_all_files(list_t *duplicated_files) {

    printf(RED);
    printf("Do you really want to delete all duplicated files (Y/N)" WHITE "\n");
    printf("(it can be fatal to delete duplicated files from root or home directory)\n");
    printf("> ");

    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    char choice;
    scanf("%c", &choice);

    if (choice == 'Y') {
        list_t *ptr = duplicated_files;
        while (ptr != NULL) {
            remove(ptr->file_data.path);
            printf("File %s removed\n", ptr->file_data.path);
            ptr = ptr->next;
        }
    }
}

void delete_file(list_t **duplicated_files, int choice) {

    list_t *temp = *duplicated_files, *prev;

    if (temp != NULL && choice == 1) {
        remove((*duplicated_files)->file_data.path);
        *duplicated_files = temp->next;
        free(temp);
        return;
    }

    for (int i = 1; i != choice; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    remove(temp->file_data.path);
    prev->next = temp->next;
    free(temp);
}

void delete_process(list_t **duplicated_files) {

    printf(CYAN "\nEnter - go to the deletion process\n(the screen will be cleared)\n>>");
    getchar();
    system("clear");

    int total_duplicate = 0;
    if (*duplicated_files == NULL) {
        printf(RED "Warn in delete flag: No duplicated files" WHITE "\n");
        return;
    } else {
        int choice;
        while (true) {
            list_t *ptr = *duplicated_files;
            if (ptr == NULL) {
                printf("All duplicated files deleted\n");
                break;
            }
            printf(YELLOW "\n");
            while (ptr != NULL) {
                ++total_duplicate;
                printf("[%d]: %s\n", total_duplicate, ptr->file_data.path);
                ptr = ptr->next;
            }

            printf(GREEN "\nEnter file number to delete (enter 0 to delete all)\n>> " WHITE);
            int val = scanf("%d", &choice);

            if (choice > total_duplicate) {
                printf("Invalid choice\n");
                return;
            } else if (val == 0) {
                return;
            } else if (choice == 0) {
                delete_all_files(*duplicated_files);
                return;
            }
            // else
            delete_file(duplicated_files, choice);

            system("clear");

            total_duplicate = 0;
        }
    }
}
