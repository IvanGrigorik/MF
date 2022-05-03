cmake_minimum_required(VERSION 3.22)
project(C C)

set(CMAKE_C_STANDARD 17)

# Add openssl
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_executable(C main.c

        # src-s
        src/parse_input.c
        src/file_work.c
        src/hashing.c)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)