#include "headers/helpful_fun.h"

int main(int argc, char *argv[]) {
    // Check to input data
    if (argc < 2) {
        output_help();
        return 0;
    }

    flags_t flags = parse_flags(argc, argv);

    // If program run with -t or -h flag
    check_flags(flags);

    // Get directory to work and create new lists to collect files info
    char *directory = get_dir(argc, argv);
    list_t *unique_files = NULL, *duplicated_files = NULL, *error_files = NULL;

    find_duplicated(directory, &unique_files, &duplicated_files, &error_files, flags);
    output_statistic(unique_files, duplicated_files, error_files, flags);

    if (flags.delete_flag) {
        delete_process(&duplicated_files);
    }

    clear_lists(&unique_files, &duplicated_files, &error_files);
}