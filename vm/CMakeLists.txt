cmake_minimum_required(VERSION 3.0) 

project(vm)

add_executable(${PROJECT_NAME} src/vm_main.c src/vm.c src/utils.c)
target_include_directories(${PROJECT_NAME} PUBLIC include) 
target_link_libraries(${PROJECT_NAME} PUBLIC vm-file)

