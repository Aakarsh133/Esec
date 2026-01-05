#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "elf_gethdr.h"


void get_Pheader(ElfView& elf){
    
    Elf32_Off e_phoff;
    Elf32_Half e_phnum;
    Elf32_Half e_phentsize;
    Elf32_Phdr *entry_head, *ph;
    
    char flags[3] = {'-','-','-'};

    e_phoff = elf.ehdr->e_phoff;
    e_phnum = elf.ehdr->e_phnum;
    e_phentsize = elf.ehdr->e_phentsize;

    entry_head = (Elf32_Phdr*)((unsigned char*)elf.addr+e_phoff);

    for(int i = 0; i<e_phnum; ++i){
        ph = &entry_head[i];
        std::string p_type;
        switch(ph->p_flags){
            case 0x7:
                flags[0] = 'R';
                flags[1] = 'W';
                flags[2] = 'X';
                break;
            case 0x6:
                flags[0] = 'R';
                flags[1] = 'W';
                break;
            case 0x5:
                flags[0] = 'R';
                flags[2] = 'X';
                break;
            case 0x4:
                flags[0] = 'R';
                break;
        }
        switch(ph->p_type){
            case 0x0:
                p_type = "NULL";
                break;
            case 0x1:
                p_type = "LOAD";
                break;             
            case 0x2:
                p_type = "DYNAMIC";
                break;
            case 0x3:
                p_type = "INTERP";
                break;
            case 0x4:
                p_type = "NOTE";
                break;
            case 0x5:
                p_type = "SHLIB";
                break;
            case 0x6:
                p_type = "PHDR";
                break;
            case 0x70000000:
                p_type = "LOPROC";
                break;
            case 0x7fffffff:
                p_type = "HIPROC";
                break;
            case PT_GNU_EH_FRAME: //0x6474e550
                p_type = "PT_GNU_EH_FRAME"; 
                break;
            case PT_GNU_STACK: //0x6474e551
                p_type = "PT_GNU_STACK";
                break;
            case PT_GNU_RELRO: //0x6474e552
                p_type = "PT_GNU_RELRO";
                break;
            default:
                p_type = "OS_SPECIFIC";
                break;
        }

        std::printf("[ %2d ] %-25s offset=%9x vaddr=%9x filesz=%9x memsz=%9x flags=%c%c%c \n",i,p_type.c_str(),ph->p_offset, ph->p_vaddr,ph->p_filesz,ph->p_memsz,flags[0],flags[1],flags[2]);
    }
}
