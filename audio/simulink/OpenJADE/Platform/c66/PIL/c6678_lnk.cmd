/****************************************************************************/
/*  sample lnk.cmd   v#####                                                 */
/*  Copyright (c) 1996@%%%%  Texas Instruments Incorporated                 */
/*                                                                          */
/*  Usage: cl6x  <src files...> -z -o <out file> -m <map file> lnk.cmd      */
/*                                                                          */
/*    Description: THIS FILE IS A SAMPLE linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C620x/C670x         */
/*                 simulator.  Use it as a guideline.  You will want to     */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/*    Notes: (1)   You must specify a directory in which your library is    */
/*                 located.  either add a -i"<directory>" line to this      */
/*                 file or use the system environment variable C_DIR to     */
/*                 specify a search path for the libraries.                 */
/*                                                                          */
/*           (2)   You may need to specify your library name with           */
/*                 -llibrary.lib if the correct library is not found        */
/*                 automatically.                                           */
/*                                                                          */
/****************************************************************************/

-heap  0x8000
-stack 0xC000
--arg_size=128 

/* Make sure these match the enumeration in ti/csl/csl_cache.h*/
#define CACHE_L1_0KCACHE  0
#define CACHE_L1_4KCACHE  1
#define CACHE_L1_8KCACHE  2
#define CACHE_L1_16KCACHE 3
#define CACHE_L1_32KCACHE 4

#define CACHE_0KCACHE   0 
#define CACHE_32KCACHE  1
#define CACHE_64KCACHE  2
#define CACHE_128KCACHE 3
#define CACHE_256KCACHE 4
#define CACHE_512KCACHE 5

MEMORY
{
        L1P_CACHE:  o = 00E00000h   l = 00008000h
        L1D_CACHE:  o = 00F00000h   l = 00008000h
        L2_CACHE:   o = 00840000h   l = 00040000h
        L2_MEM:     o = 00800000h   l = 00040000h
        MSM_MEM:    o = 0C000000h   l = 00400000h
        EXT_MEM:    o = 80000000h   l = 20000000h
}

/*
* There are two basic types of sections:
* $ Initialized sections contain data or executable code. The C/C++ compiler creates the following
* initialized sections:
* - The .args section contains the command argument for a host-based loader. This section is
* read-only. See the --arg_size option for details.
* - For EABI only, the .binit section contains boot time copy tables. For details on BINIT, see the
* TMS320C6000 Assembly Language Tools User's Guide for linker command file information.
* - For COFF ABI only, the .cinit section contains tables for initializing variables and constants.
* - The .pinit section for COFF ABI, or the .init_array section for EABI, contains the table for calling
* global constructor tables.
* - For EABI only, the .c6xabi.exidx section contains the index table for exception handling. The
* .c6xabi.extabsection contains un-winded instructions for exception handling. These sections are
* read-only. See the --exceptions option for details.
* - The .name.load section contains the compressed image of section name. This section is
* read-only. See the TMS320C6000 Assembly Language Tools User's Guide for information on copy
* tables.
* - The .ppinfo section contains correlation tables and the .ppdata section contains data tables for
* compiler-based profiling. See the --gen_profile_info option for details.
* - The .const section contains string literals, floating-point constants, and data defined with the
* C/C++ qualifiers far and const (provided the constant is not also defined as volatile).
* - For EABI only, the .fardata section reserves space for non-const, initialized far global and static
* variables.
* - For EABI only, the .neardata section reserves space for non-const, initialized near global and
* static variables.
* - For EABI only, the .rodata section reserves space for const near global and static variables.
* - The .switch section contains jump tables for large switch statements.
* - The .text section contains all the executable code.
* 
* $ Uninitialized sections reserve space in memory (usually RAM). A program can use this space at run
* time to create and store variables. The compiler creates the following uninitialized sections:
* - For COFF ABI only, the .bss section reserves space for global and static variables. When you
* specify the --rom_model linker option, at program startup, the C boot routine copies data out of the
* .cinit section (which can be in ROM) and stores it in the .bss section. The compiler defines the
* global symbol $bss and assigns $bss the value of the starting address of the .bss section.
* - For EABI only, the .bss section reserves space for uninitialized global and static variables.
* - The .far section reserves space for global and static variables that are declared far.
* - The .stack section reserves memory for the system stack.
* - The .sysmem section reserves space for dynamic memory allocation. The reserved space is used
* by dynamic memory allocation routines, such as malloc, calloc, realloc, or new. If a C/C++ program
* does not use these functions, the compiler does not create the .sysmem section.
*/

SECTIONS
{
    .dummy: 
    {
      cache_l1p_size = CACHE_L1_32KCACHE;
      cache_l1d_size = CACHE_L1_32KCACHE;
      cache_l2_size = CACHE_256KCACHE;
    } 
    
    .text       >       L2_MEM

    .sysmem     >       MSM_MEM
    .stack      >       L2_MEM
    .args       >       EXT_MEM

    GROUP
    {
            .neardata   /* Move .bss after .neardata and .rodata.  ELF allows */
            .rodata     /* uninitialized data to follow initialized data in a */
            .bss        /* single segment. This order facilitates a single    */
                        /* segment for the near DP sections.                  */
    }>  L2_MEM

    .cinit      >       EXT_MEM
    
    .cio        >       EXT_MEM
    .data       >       L2_MEM
    .switch     >       L2_MEM
    .fardata    >       L2_MEM
    
    .const      >>      L2_MEM | MSM_MEM
    .far        >>      L2_MEM | MSM_MEM
    .oj_ext_data >>     MSM_MEM | EXT_MEM
}

