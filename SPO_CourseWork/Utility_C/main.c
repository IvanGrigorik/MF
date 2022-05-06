#include "headers/parse_input.h"
#include "headers/structures.h"
#include "headers/file_work.h"
#include "headers/testing.h"

int main(int argc, char *argv[]) {
    // Parse flags from terminal input
    flags_t flags = parse_flags(argc, argv);

    // If program run with -t flag
    if (flags.test_flag) {
        // Test-block
        full_test();

        return 0;
    }

    // Get directory to work
    char *directory = get_dir(argc, argv);

    list_t *unique_files = NULL;
    list_t *duplicated_files = NULL;
    list_t *error_files = NULL;
    find_duplicated(directory, &unique_files, &duplicated_files, &error_files, flags);

    output_statistic(unique_files, duplicated_files, error_files, flags);
    if(flags.delete_flag){
        delete_files(&duplicated_files);
    }
}