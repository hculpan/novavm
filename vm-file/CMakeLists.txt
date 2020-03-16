cmake_minimum_required(VERSION 3.0) 

project(vm-file)

add_library(${PROJECT_NAME} src/vm-file.c) 
target_include_directories(${PROJECT_NAME} PUBLIC include)