Use  "g++ main.cpp src/elf_load.cpp  src/elf_gethdr.cpp ./src/elf_check.cpp -Iinclude -o main" or equivalent for compilation.

Checksec like tool implemented in C/C++

This version works for ELf32 Binaries on Little Endianness, adding support for ELf64 is logically very similar 

This project helped me revised my Pwning/Reversing experience and taught me to effeciently navigate through elfs and disassemblers, figuring out the neccessary pointer arthimatic was yet the most interesting part for me. Feel free to review and suggest improvements and addtions.

I have an intuition that the core parsing logic of this project can be expanded into fully fledged Disassemblers as well.

Summarisation of core logics->

NX -> Absence of PT_GNU_STACK is considered as NX=1 in nearly all modern machines.
Else, PT_GNU_STACK should have no execute permissions.

RELRO -> Absence of PT_GNU_RELRO means RELRO.
Presence indicates atleast Partial RELRO.
If DT_BIND_NOW exists in Dynamic Segment then GOT table is written before the control transfer to User Hence Full RELRO

PIE -> Check E_TYPE in Elf header it should be ET_DYN

Canary -> Most confusing initially you need to validate whether _stack_chk_fail symbol exists, I found a way to determine this, it usually is linked through PLT hence you need to check REL.PLT where if it exists it links you to .dynsym and eventually String Symbol table where it is written.