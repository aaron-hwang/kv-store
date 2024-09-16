#include <stdio.h>

#include "hashmap.h"

int main(void) {
    Hashmap map;
    initHashmap(&map);
    setValue(&map, "test", 3);
    printf("%d\n", getValue(&map, "test"));
    deleteKey(&map, "test");
    printf("%d\n", getValue(&map, "test"));
    setValue(&map, "test0", 4);
    setValue(&map, "test1", 5);
    setValue(&map, "test2", 6);
    setValue(&map, "test3", 7);
    setValue(&map, "test4", 8);
    setValue(&map, "test5", 9);
    setValue(&map, "test6", 10);
    setValue(&map, "test7", 4);
    setValue(&map, "test8", 5);
    setValue(&map, "test9", 6);
    setValue(&map, "test10", 7);
    setValue(&map, "test11", 8);
    setValue(&map, "test12", 9);
    setValue(&map, "test13", 10);
    setValue(&map, "test14", 4);
    setValue(&map, "test15", 5);
    setValue(&map, "test16", 6);
    setValue(&map, "test17", 7);
    setValue(&map, "test18", 8);
    setValue(&map, "test19", 9);
    setValue(&map, "test20", 10);
    dumpHashmap(&map);
    deleteHashmap(&map);
    return 0;
}
