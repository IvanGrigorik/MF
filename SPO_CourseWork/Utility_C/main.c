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


    node_t *unique_files = (node_t *) malloc(sizeof(node_t));
    list_t *duplicated_files = (list_t *) malloc(sizeof(list_t));
    unique_files->left = NULL;
    unique_files->right = NULL;
    duplicated_files->next = NULL;

    collect_files(directory, &unique_files, &duplicated_files, flags);
//    files_t *unique_files = (files_t *) malloc(sizeof(files_t));
//    unique_files->num = 0;
//    files_t *duplicated_files = (files_t *) malloc(sizeof(files_t));
//    duplicated_files->num = 0;
//
//    collect_files(directory, &unique_files, &duplicated_files, flags);
//
//    for (int i = 0; i < unique_files->num; i++) {
//        printf(GREEN "File name: %s\n", unique_files->file_data[i].filename);
//    }
//    printf("\n\n\n");
//    for (int i = 0; i < duplicated_files->num; i++) {
//        printf(YELLOW "File name: %s\n", duplicated_files->file_data[i].filename);
//    }
//    printf(WHITE);

}