#pragma once
#include "elf_view.h"

bool load_Elf(int fd, ElfView& elf);
void unload_Elf(ElfView& elf);
