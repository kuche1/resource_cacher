cmake_minimum_required(VERSION 3.25.2)

project(resource_cacher)

##### TODO delete #set(CMAKE_C_FLAGS "-Werror -Wextra -Wall -pedantic -Wunused-result -Wconversion")
add_compile_options(-Werror -Wextra -Wall -pedantic -Wunused-result)

add_library(${PROJECT_NAME} ${PROJECT_NAME}.c)
add_library(sha-256 sha-2/sha-256.c) # TODO this is wrong
add_library(utils utils.c)

add_executable(test tests.c)

target_link_libraries(test ${PROJECT_NAME} sha-256 utils)
