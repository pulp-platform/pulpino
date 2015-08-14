SEARCH_DIR(.)
__DYNAMIC  =  0;

MEMORY
{
    ram         : ORIGIN = 0x00000000, LENGTH = 0x07000
    stack       : ORIGIN = 0x00000000, LENGTH = 0x01000
}

/* Stack information variables */
_min_stack      = 0x1000;   /* 4K - minimum stack space to reserve */


/* We have to align each sector to word boundaries as our current s19->slm
 * conversion scripts are not able to handle non-word aligned sections. */

SECTIONS
{
    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
    } > ram
        
    .text : {
        . = ALIGN(4);
        _stext = .;
        *(.text)
        _etext  =  .;
        __CTOR_LIST__ = .;
        LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
        *(.ctors)
        LONG(0)
        __CTOR_END__ = .;
        __DTOR_LIST__ = .;
        LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
        *(.dtors)
        LONG(0)
        __DTOR_END__ = .;
        *(.lit)
        *(.shdata)
        _endtext = .;
    }  > ram
    
    .rodata : {
        . = ALIGN(4);
        *(.rodata);
        *(.rodata.*)
    } > ram

    /* or1k-specific */
    .got : { *(.got.plt) * (.igot.plt) *(.got) *(.igot) } > ram
    .shbss :
    {
        . = ALIGN(4);
        *(.shbss)
    } > ram
    
    .talias :
    { 
    }  > ram
    
    .data : {
        . = ALIGN(4);
        sdata  =  .;
        _sdata  =  .;
        *(.data);
        *(.data.*)
        edata  =  .;
        _edata  =  .;
    } > ram
    
    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        _bss_end = .;
    } > ram
    
    /* ensure there is enough room for stack */
    .stack (NOLOAD): {
        . = ALIGN(4);
        . = . + _min_stack ;
        . = ALIGN(4);
        stack = . ;
        _stack = . ;
    } > stack
    
    .stab  0 (NOLOAD) :
    {
        [ .stab ]
    }
    
    .stabstr  0 (NOLOAD) :
    {
        [ .stabstr ]
    }
}

