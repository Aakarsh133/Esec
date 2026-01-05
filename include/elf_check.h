#pragma once
#include "elf_view.h"

bool has_nx(const ElfView& elf);
bool is_pie(const ElfView& elf);
bool has_relro(const ElfView& elf);