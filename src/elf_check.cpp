#include <iostream>
#include <elf.h>
#include <cstring>

#include "elf_check.h"

bool has_canary(ElfView& elf){
    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_shoff), *dynsym_base=nullptr;
    unsigned char *strtbl_base=nullptr;
    off_t dyn_off, str_tbl;

    elf.res.Canary = RED;
    elf.res.Canary += "No Canary Found";

    /*Selfnote - Cast everything to unsigned char*
    before doing pointer arthimatic */

    Elf32_Shdr *sh;
    Elf32_Sym *dynsym;
    
    for (int i = 0; i < elf.ehdr->e_shnum; ++i) { 
        /*Here I took the offsets of dynsym and str table*/
        sh = (Elf32_Shdr*)(base + i * elf.ehdr->e_shentsize);

        if (sh->sh_type == SHT_DYNSYM) {
            dynsym_base = (unsigned char*)elf.addr + sh->sh_offset;
            dyn_off = sh->sh_offset;

            Elf32_Shdr *str_sh = (Elf32_Shdr*)(base + sh->sh_link * elf.ehdr->e_shentsize);
            strtbl_base = (unsigned char*)elf.addr + str_sh->sh_offset;

            break;
        }
    }

    if (!dynsym_base || !strtbl_base)
        return false;

    for (int i = 0; i<elf.ehdr->e_shnum; ++i){
        sh = (Elf32_Shdr*)(base + i * elf.ehdr->e_shentsize);
        if ((sh->sh_type == SHT_REL)){
            unsigned char *rel_base = ((unsigned char*)elf.addr + sh->sh_offset);
            int j = 0;
            Elf32_Rel *rel;
            
            while((j*sizeof(Elf32_Rel)+ rel_base) < (rel_base + sh->sh_size)){
                rel = (Elf32_Rel*)(rel_base + j*sizeof(Elf32_Rel));
                uint32_t sym_index = ELF32_R_SYM(rel->r_info);
                dynsym = (Elf32_Sym*)(((unsigned char*)elf.addr+ dyn_off + sym_index * sizeof(Elf32_Sym)));

                const char *name = (const char*)(strtbl_base + dynsym->st_name);                
                if (!strcmp(CANARY,name)){
                    elf.res.Canary = GREEN;
                    elf.res.Canary += "Canary Found";
                    return true;
                }
                j++;

            }
            
        }
    }
    return false;

}

bool has_nx(ElfView& elf){

    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff); 
    elf.res.NX = GREEN;
    elf.res.NX += "NX Enabled";

    Elf32_Phdr *ph;

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if ((ph->p_type==PT_GNU_STACK)){
            if (!(ph->p_flags & PF_X)){
                return true;
            }else{
                elf.res.NX = RED;
                elf.res.NX += "NX DISABLED";
                return false;
            }
        }
    }
    return true; /*For modern machines NX is enabled but for very old machines 90ish stack is disabled
    if no PT_GNU_STACK is present*/

}

bool is_pie(ElfView& elf){
    elf.res.PIE = RED;
    elf.res.PIE += "PIE DISABLED";
    if(elf.ehdr->e_type == ET_DYN){
        elf.res.PIE = GREEN;
        elf.res.PIE += "PIE ENABLED";
        return true;
    }
    return false;
}

bool has_relro(ElfView& elf){
    unsigned char *base = ((unsigned char*)elf.addr + elf.ehdr->e_phoff), *dyn_base;
    Elf32_Phdr *ph;
    Elf32_Dyn *dyn;

    elf.res.RELRO = RED;
    elf.res.RELRO += "NONE";

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if (ph->p_type==PT_DYNAMIC){
            dyn_base = ((unsigned char*)elf.addr + ph->p_offset);
        }
    }

    for(int i = 0; i<elf.ehdr->e_phnum; ++i){
        ph = (Elf32_Phdr*)(base + i * elf.ehdr->e_phentsize);
        if (ph->p_type==PT_GNU_RELRO){
            elf.res.RELRO = YELLOW;
            elf.res.RELRO += "PARTIAL RELRO";
            if ((dyn != NULL)){ //FIXED
               int j = 0;
               while(j*sizeof(Elf32_Dyn)+dyn_base < (unsigned char*)elf.addr+ph->p_offset+ ph->p_filesz){
                    dyn = (Elf32_Dyn*)(dyn_base + j * sizeof(Elf32_Dyn));
                    if (dyn->d_tag == DT_BIND_NOW || (dyn->d_tag == DT_FLAGS_1 && 
                        (dyn->d_un.d_val & DF_1_NOW))){ 
                            /* For me 0X1e worked but for what i understand
                                d_tag here is a bit mask of multiple flags, so this is a generic fix
                            */
                        elf.res.RELRO = GREEN;
                        elf.res.RELRO += "FULL RELRO";
                        return true;
                    }
                    j++;
               }
            }
        }
    }
    return false;

}