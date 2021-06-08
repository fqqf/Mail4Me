cmake_minimum_required(VERSION 3.15)
project(mail4me C)

set(CMAKE_C_STANDARD 99)

#add_executable(mail4me main.c core/jsmn_parser.h main2.c)
add_executable(mail4me core/api.h core/jsmn_parser.h main.c)