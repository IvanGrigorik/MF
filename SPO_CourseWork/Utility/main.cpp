#include "functions.h"

int main(int argc, char *argv[]) {


    try {
        std::string dir_to_find = (argc == 1 || argv[1][0] == '-') ? "." : argv[1];
        flags_t flags;
        flags = parse_flags(argc, argv);

        std::cout << "Directory to find duplicate files: " << dir_to_find << std::endl << std::endl;

        std::vector<file_data_t> unique_files{};
        std::vector<file_to_delete_t> duplicated_files{};

        collect_files(dir_to_find, unique_files, duplicated_files, flags);

        files_output(unique_files, duplicated_files, flags);

        if (flags.delete_flag) {
            delete_files(duplicated_files, flags);
        }
    } catch (std::exception &e) {
        std::cout << e.what();
    }

    return 0;
}
