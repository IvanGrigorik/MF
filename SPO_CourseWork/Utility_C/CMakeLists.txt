cmake_minimum_required(VERSION 3.22)
project(C C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_C_STANDARD 17)

# Add openssl
find_package(OpenSSL REQUIRED)

add_executable(C main.c

        # src-s
        src/parse_input.c
        src/file_work.c
        src/hashing.c
        src/testing.c
        src/helpful_fun.c)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)