cmake_minimum_required(VERSION 3.28)
project(kv_store C)

set(CMAKE_C_STANDARD 23)

add_executable(kv_store main.c
        kvstore.h
        hashmap.h
        hashmap.c)
