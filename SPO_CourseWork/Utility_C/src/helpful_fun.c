//
// Created by sifi on 5/8/22.
//

#include "../headers/helpful_fun.h"

void check_flags(flags_t flags) {

    if (flags.test_flag) {
        // Test-block
        full_test();
    }
    if (flags.help_flag) {
        // Show helpful info
        output_help();
    }
}

void output_help() {
    // Glad if you help me do it in other way
    printf("Usage: ./C [FLAGS..] [DIRECTORY..]\n\n");
    printf("FLAGS: \n");
    printf(" Controlling formatted output:\n");
    printf("  -h    Output helpful information\n");
    printf("  -s    Output extended statistic (warnings and errors messages)\n");
    printf("\n Controlling collecting process:\n");
    printf("  -n    Collect files with same content, but different names\n");
    printf("  -a    Collect hidden files (starting with \".\")\n");
    printf("\n Other processes: \n");
    printf("  -d     Delete duplicated files with selection\n");
    printf("  -t     Test all program modules\n");
    printf("\n Other information: \n");
    printf("         Program launching from the root directory is redirected to the\n");
    printf("         home directory for system integrity\n");
    printf("\nReports bugs to givan502@gmail.com\n");
    exit(EXIT_SUCCESS);
}