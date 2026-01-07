Use  "g++ main.cpp src/elf_load.cpp  src/elf_gethdr.cpp ./src/elf_check.cpp -Iinclude -o main" or equivalent for compilation.

Checksec like tool implemented in C/C++

This version works for ELf32 Binaries on Little Endianness, adding support for ELf64 is logically very similar 

This project helped me revised my Pwning/Reversing experience and taught me to effeciently navigate through elfs and disassemblers, figuring out the neccessary pointer arthimatic was yet the most interesting part for me. Feel free to review and suggest improvements and addtions.

I have an intuition that the core parsing logic of this project can be expanded into fully fledged Disassemblers as well.
