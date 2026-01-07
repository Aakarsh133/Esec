#pragma once

#include <elf.h>
#include <cstddef>

struct ElfRes{
    std::string NX;
    std::string RELRO;
    std::string PIE;
    std::string Canary;
};

struct ElfView {
    void *addr;
    size_t size;
    Elf32_Ehdr* ehdr;
    ElfRes res;
};

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

#define CANARY "__stack_chk_fail"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_func_error(msg) \
    do { perror(msg); return NULL; } while (0)
