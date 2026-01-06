#include <iostream>
#include <elf.h>

#include "elf_check.h"

bool has_nx(ElfView& elf){

    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff); 
    elf.res.NX = "NX Enabled";

    /*Selfnote - Cast everything to unsigned char*
    before doing pointer arthimatic */

    Elf32_Phdr *ph;

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if ((ph->p_type==PT_GNU_STACK)){
            if (!(ph->p_flags & PF_X)){
                return true;
            }else{
                elf.res.NX = "NX Disabled";
                return false;
            }
        }
    }
    return true; /*For modern machines NX is enabled but for very old machines 90ish stack is disabled
    if no PT_GNU_STACK is present*/

}

bool is_pie(ElfView& elf){
    elf.res.PIE = "PIE DISABLED";
    if(elf.ehdr->e_type == ET_DYN){
        elf.res.PIE = "PIE ENABLED";
        return true;
    }
    return false;
}

bool has_relro(ElfView& elf){
    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff);
    Elf32_Phdr *ph;
    Elf32_Dyn *dyn;

    elf.res.RELRO = "NONE";

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if (ph->p_type==PT_DYNAMIC){
            dyn = (Elf32_Dyn*)((unsigned char*)elf.addr + ph->p_offset);
        }
    }

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if (ph->p_type==PT_GNU_RELRO){
            elf.res.RELRO = "PARTIAL";
            if ((dyn != NULL)&&(dyn->d_tag != DT_BIND_NOW)){ //FIX
                elf.res.RELRO = "FULL";
            }
        }
    }
    return false;

}