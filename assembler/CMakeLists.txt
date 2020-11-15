cmake_minimum_required(VERSION 3.0) 

project(assembler)

add_executable(${PROJECT_NAME} src/assembler_main.c src/util.c)
target_include_directories(${PROJECT_NAME} PUBLIC include) 
target_link_libraries(${PROJECT_NAME} PUBLIC vm-file)

