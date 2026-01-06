#pragma once
#include "elf_view.h"

bool has_nx(ElfView& elf);
bool is_pie(ElfView& elf);
bool has_relro(ElfView& elf);