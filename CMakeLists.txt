cmake_minimum_required(VERSION 2.6)
project(sash)
include_directories(.)
add_executable(sash main.c main.h command.c command.h global.h parser.c
parser.h utility.c utility.h environ.c environ.h ls.c ls.h)
install (TARGETS sash DESTINATION bin)
