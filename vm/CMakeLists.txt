cmake_minimum_required(VERSION 3.0) 

project(vm)

add_executable(${PROJECT_NAME} src/vm.c) 
target_link_libraries(${PROJECT_NAME} PUBLIC vm-file)