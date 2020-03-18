cmake_minimum_required(VERSION 3.0)

project(novavm)

add_subdirectory(vm)
add_subdirectory(assembler)
add_subdirectory(vm-file) 

enable_testing()
