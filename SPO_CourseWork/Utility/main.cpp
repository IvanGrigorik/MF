#include "functions.h"
#include "test_launch.h"

int main(int argc, char *argv[]) {


    try {
        flags_t flags;
        flags = parse_flags(argc, argv);

        std::vector<file_data_t> unique_files{};
        std::vector<file_to_delete_t> duplicated_files{};

        if (flags.test_flag) {
            generate_files();
            // Wait to show consists of all files
            std::cin.get();
            show_search(unique_files, duplicated_files);

            return 0;
        }

        std::string dir_to_find = get_dir(argc, argv);

        find_duplicated(dir_to_find, unique_files, duplicated_files, flags);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
