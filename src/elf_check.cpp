#include <iostream>
#include <elf.h>

#include "elf_check.h"

bool has_nx(const ElfView& elf){

    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff); 

    /*Selfnote - Cast everything to unsigned char*
    before doing pointer arthimatic */

    Elf32_Phdr *ph;

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if ((ph->p_type==PT_GNU_STACK)){
            return !(ph->p_flags & PF_X);
        }
    }
    return true; /*For modern machines NX is enabled but for very old machines 90ish stack is disabled
    if no PT_GNU_STACK is present*/

}

bool is_pie(const ElfView& elf){
    return (elf.ehdr->e_type == ET_DYN);
}

bool has_relro(const ElfView& elf){
    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff);
    Elf32_Phdr *ph;

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if (ph->p_type==PT_GNU_RELRO) return true;
    }
    return false;

}