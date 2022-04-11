//
// Created by sifi on 11.04.2022.
//

#include "test_launch.h"


void generate_files() {
    srand(time(NULL));

    std::cout << "Creating new files... " << std::endl;

    std::string file_name = "/home/sifi/BSUIR/MF/SPO_CourseWork/Utility/files/file";
    char content[4];

    FILE *fp;

    for (int i = 0; i < 9; i++) {
        file_name.push_back(i + 48);

        fp = fopen(file_name.c_str(), "w");

        for (int j = 0; j < 3; j++) {
            content[j] = (rand() % 2) + 48;
        }
        content[3] = '\n';

        std::cout << "File name: " << "file" << i << "; File content: " << content;

        fputs(content, fp);
        fclose(fp);
        file_name.pop_back();

    }
}

void output_file_content() {
    std::string file_name = "/home/sifi/BSUIR/MF/SPO_CourseWork/Utility/files/file";
    FILE *fp;
    char content[4];

    std::cout << std::endl;
    for (int i = 0; i < 9; i++) {
        file_name.push_back(i + 48);

        fp = fopen(file_name.c_str(), "r");

        fgets(content, 4, fp);
        std::cout << "File name: " << "file" << i << "; File content: " << content << std::endl;

        fclose(fp);
        file_name.pop_back();
    }
}

void show_search(std::vector<file_data_t> &unique_files,
                 std::vector<file_to_delete_t> &duplicated_files) {

    std::string dir_to_find = "/home/sifi/BSUIR/MF/SPO_CourseWork/Utility/files";

    flags_t flags;
    // Show default search without parameters
    output_file_content();
    find_duplicated(dir_to_find, unique_files, duplicated_files, flags);
    std::cin.get();
    system("clear");

    unique_files.clear();
    duplicated_files.clear();

    // Show search with statistic output
    output_file_content();
    flags.stats = true;
    find_duplicated(dir_to_find, unique_files, duplicated_files, flags);
    std::cin.get();
    system("clear");
    flags.stats = false;

    unique_files.clear();
    duplicated_files.clear();

    // Show search with name filter
    output_file_content();
    flags.name_flag = true;
    find_duplicated(dir_to_find, unique_files, duplicated_files, flags);
    std::cin.get();
    system("clear");

    unique_files.clear();
    duplicated_files.clear();
}