#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

#include "elf_load.h"
#include "elf_view.h"


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_func_error(msg) \
    do { perror(msg); return NULL; } while (0)

bool load_Elf(int fd, ElfView& elf){
    struct stat sb;

    if (fstat(fd, &sb) == -1){
        handle_func_error("fstat");
    }

    elf.addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (elf.addr == MAP_FAILED) handle_func_error("mmap");

    elf.ehdr = (Elf32_Ehdr*)elf.addr;
    elf.size = sb.st_size;

    if ((elf.ehdr->e_ident[0] != 0x7f) || (elf.ehdr->e_ident[1] != 0x45) || (elf.ehdr->e_ident[2] != 0x4c) || (elf.ehdr->e_ident[3] != 0x46)){
        std::cerr << "NOT ELF!";
        return false;
    }

    if (elf.ehdr->e_ident[EI_CLASS] != ELFCLASS32){
        std::cerr << "ONLY x86 allowed for now!";
        return false;
    }

    return true;
}
