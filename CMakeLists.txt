cmake_minimum_required(VERSION 3.15)
project(mail4me C)

set(CMAKE_C_STANDARD 99)

#add_executable(mail4me main.c lib/jsmn_parser.h main2.c)
add_executable(mail4me main2.c lib/jsmn_parser.h)