#pragma once

#include <elf.h>
#include <cstddef>

struct ElfView {
    void *addr;
    size_t size;
    Elf32_Ehdr* ehdr;
};


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_func_error(msg) \
    do { perror(msg); return NULL; } while (0)
