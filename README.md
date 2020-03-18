# NovaVM
A simple VM.

This project is just a little VM for me to play around with.  My intention is to write a VM (probably stack-based, but honestly haven't quite made up my mind), and assembler, and a high-level language compiler.  All will initially be written in C with a goal of making them as platform-independent as possible.  My longer-term goal will be to re-implement the compile (and maybe assembler) in my high-level language, making it self-hosting.  

The reason I say "maybe" on the assembler is because it shares a library with the core VM dealing with the .NVM file format.  This way, I can be sure I keep it in sync between the core project and the assembler.  Since I plan to have my compiler output assembler code, this will also keep my compiler up to date with any file format changes.  If I were to rewrite the assembler in my high-level language, then the change of keeping it in sync with the core VM is much lower.

## Features of the VM
* Stack-based
* Dynamic memory allocation with some sort of garbage collection or reference counting


## Requirements:
* Standard C build tools (gcc/clang, make)
* CMake 3.16+


## To Build:
* From the root directory: `cmake .`
* Assuming above is successful: `make`


## Where does the name 'NovaVM' come from?
The name comes from one of my all-time favorite comic book super-heroes, Nova.  Introduced in 1976, I didn't actually read his comic until several years later, after his initial run had already been canceled.  I was given issue #1 by a comic collector as a teenager around 1980, and it took me quite some time as a teenager to collect all 25 of the original issues.  Since then, he has remained one of my favorite characters, although it was his initial run that I still like best. 