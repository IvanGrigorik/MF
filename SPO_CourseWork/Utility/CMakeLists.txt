cmake_minimum_required(VERSION 3.21)
project(SPO_CourseWork)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_executable(SPO_CourseWork main.cpp functions.cpp functions.h test_launch.h test_launch.cpp)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)