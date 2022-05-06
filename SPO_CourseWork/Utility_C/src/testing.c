//
// Created by sifi on 5/5/22.
//

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../headers/testing.h"
#include "../headers/structures.h"
#include "../headers/file_work.h"


void full_test(){
    generate_files();

    test_duplicated();
}

void generate_files() {
    srand(time(NULL));

    printf("Creating new files... \n");

    char file_name[FILENAME_MAX] = "/home/sifi/BSUIR/MF/SPO_CourseWork/Utility_C/files/file";
    char content[4];

    FILE *fp;

    for (int i = 0; i < 9; i++) {
        file_name[55] = (char)(i+48);

        fp = fopen(file_name, "w");

        for (int j = 0; j < 3; j++) {
            content[j] = (char)((rand() % 2) + 48);
        }
        content[3] = '\0';

        printf("\nFile name: %s\nFile content: %s", file_name, content);

        fputs(content, fp);
        fclose(fp);
    }
}


void test_duplicated(){
    char dir[] = "/home/sifi/BSUIR/MF/SPO_CourseWork/Utility_C/files";
    flags_t flags;
    flags.stats = true;
    list_t *unique_files = NULL;
    list_t *duplicated_files = NULL;
    list_t *error_files = NULL;
    find_duplicated(dir, &unique_files, &duplicated_files, &error_files, flags);

    output_statistic(unique_files, duplicated_files, error_files, flags);

}