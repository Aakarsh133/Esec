#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/elf_view.h"
#include "include/elf_load.h"
#include "include/elf_gethdr.h"
// namespace std



int main(int argc, char *argv[]){

    int fd;
    ElfView elf;

    if (argc != 2){
        std::cerr << argv[0] << "file\n";
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) handle_error("open");

    if (load_Elf(fd, elf)) {
        std::cout<<"ELF FOUND!" << std::endl;
    }
    else exit(EXIT_FAILURE);

    get_Pheader(elf);
    exit(EXIT_SUCCESS);

}