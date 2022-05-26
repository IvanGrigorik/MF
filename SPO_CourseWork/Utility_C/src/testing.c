//
// Created by sifi on 5/5/22.
//

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../headers/testing.h"
#include "../headers/file_work.h"
#include "../headers/parse_input.h"

char dir[] = "files";

void full_test() {
    generate_files();
    getchar();

    test_duplicated();
    exit(EXIT_SUCCESS);
}

void generate_files() {

    srand(time(NULL));

    printf("Creating new files... \n");

    char file_name[FILENAME_MAX] = "files/file";
    char content[4];

    FILE *fp;

    for (int i = 0; i < 9; i++) {
        file_name[10] = (char) (i + 48);

        fp = fopen(file_name, "rw+");

        for (int j = 0; j < 3; j++) {
            content[j] = (char) ((rand() % 2) + 48);
        }
        content[3] = '\0';

        printf("\nFile name: %s\nFile content: %s", file_name, content);

        fputs(content, fp);
        fclose(fp);
    }
}

void clear_lists(list_t **unique_files, list_t **duplicated_files, list_t **error_files) {

    list_t *ptr;
    while (*unique_files != NULL) {
        ptr = *unique_files;
        *unique_files = (*unique_files)->next;
        free(ptr);
    }

    while (*duplicated_files != NULL) {
        ptr = *duplicated_files;
        *duplicated_files = (*duplicated_files)->next;
        free(ptr);
    }

    while (*error_files != NULL) {
        ptr = *error_files;
        *error_files = (*error_files)->next;
        free(ptr);
    }

}

void test_duplicated() {

    flags_t flags;
    init_flags(&flags);

    list_t *unique_files = NULL, *duplicated_files = NULL, *error_files = NULL;

    // Common files founding (without flags)
    printf(MAGENTA "\nCommon test without flags\n" WHITE);
    find_duplicated(dir, &unique_files, &duplicated_files, &error_files, flags);
    output_statistic(unique_files, duplicated_files, error_files, flags);
    clear_lists(&unique_files, &duplicated_files, &error_files);

    // Delete all duplicated files
    printf(MAGENTA "\n\nTest with deletion flags\n" WHITE);
    flags.delete_flag = true;
    find_duplicated(dir, &unique_files, &duplicated_files, &error_files, flags);
    output_statistic(unique_files, duplicated_files, error_files, flags);

    printf(MAGENTA "\n\nDelete 1st file \n" WHITE);
    delete_file(&duplicated_files, 1);
    output_statistic(unique_files, duplicated_files, error_files, flags);

    clear_lists(&unique_files, &duplicated_files, &error_files);
}

void delete_files(){

    printf("Delete all files... \n");

    char file_name[FILENAME_MAX] = "files/file";

    for (int i = 0; i < 9; i++) {
        file_name[10] = (char) (i + 48);
        remove(file_name);
    }
    rmdir(dir);
}