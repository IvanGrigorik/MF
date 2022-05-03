#include "include/parse_input.h"
#include "include/structures.h"
#include "include/file_work.h"

int main(int argc, char *argv[]) {
    // Parse flags from terminal input
    flags_t flags = parse_flags(argc, argv);

    // If program run with -t flag
    if (flags.test_flag) {
        // Test-block

        return 0;
    }

    // Get directory to work
    char *directory = get_dir(argc, argv);


    list_t *unique_files = NULL;
    list_t *duplicated_files = NULL;

    collect_files(directory, &unique_files, &duplicated_files, flags);
    list_t *runner;
    runner = unique_files;
    while (1) {
        printf(GREEN "%s\n", runner->file_data.filename);
        if (runner->next) {
            runner = runner->next;
        } else {
            break;
        }
    }
    runner = duplicated_files;
    while (1) {
        printf(YELLOW "%s\n", runner->file_data.filename);
        if (runner->next) {
            runner = runner->next;
        } else {
            break;
        }
    }
//    for (int i = 0; i < unique_files->num; i++) {
//        printf(GREEN "File name: %s\n", unique_files->file_data[i].filename);
//    }
//    printf("\n\n\n");
//    for (int i = 0; i < duplicated_files->num; i++) {
//        printf(YELLOW "File name: %s\n", duplicated_files->file_data[i].filename);
//    }
//    printf(WHITE);

}