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

    output_statistic(unique_files, duplicated_files, flags);
}