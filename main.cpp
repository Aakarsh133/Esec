#include <iostream>
#include <elf.h>
#include <fcntl.h>

#include "include/elf_view.h"
#include "include/elf_load.h"
#include "include/elf_gethdr.h"
#include "include/elf_check.h"

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

    //get_Pheader(elf);
    std::cout << has_nx(elf) << std::endl;
    
    exit(EXIT_SUCCESS);

}