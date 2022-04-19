cmake_minimum_required(VERSION 3.22)
project(Utility_C C)

set(CMAKE_C_STANDARD 11)

add_executable(Utility_C
        main.c

        # src-s
        src/parse_input.c
        src/file_work.c
        src/hashing.c

        # functions headers
        include/parse_input.h
        include/file_work.h
        include/hashing.h

        # structures
        include/structures.h)