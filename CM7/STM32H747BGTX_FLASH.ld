/*******************************************************************/
/* MiniConsole V3 - Board Support Package - Linker script          */
/*                                                                 */
/* Author: Marek Ryn                                               */
/* Version: 0.1b                                                   */
/*                                                                 */
/* Changelog:                                                      */
/*                                                                 */
/* - 0.1b	- Development version                                  */
/*******************************************************************/


/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(OS_RAM) + LENGTH(OS_RAM); 

_Min_Heap_Size = 0x200; /* required amount of heap  */
_Min_Stack_Size = 0x1000; /* required amount of stack */

/* Memories definition */
MEMORY
{
  ITCMRAM	(xrw)	: ORIGIN = 0x00000000, 	LENGTH = 64K	/* Fast memory for CM7 Core - Instructions */
  DTCMRAM 	(rw)  	: ORIGIN = 0x20000000, 	LENGTH = 128K   /* Fast memory for CM7 Core - Data */
  
  OS_FLASH	(rx)	: ORIGIN = 0x08000000, 	LENGTH = 512K   /* Operating system code */
  OS_RAM	(xrw)	: ORIGIN = 0x24000000, 	LENGTH = 128K   /* Operating system data */
  
  SH0_RAM	(rw)	: ORIGIN = 0x30040000, 	LENGTH = 32K    /* Shared memory for Sound System */
  SH1_RAM	(rw)	: ORIGIN = 0x38008000, 	LENGTH = 32K    /* Shared memory for application */
  
  BUF_RAM	(rw)	: ORIGIN = 0x38000000, 	LENGTH = 32K    /* Uncached memory for DMA buffers */
  BK_RAM	(rw)	: ORIGIN = 0x38800000, 	LENGTH = 4K     /* Configuration data */
}

/* Sections */
SECTIONS
{
  /* The startup code into "OS_FLASH" memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >OS_FLASH

  /* The program code and other data into "OS_FLASH" memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >OS_FLASH

  /* Used by the startup to initialize copy routine*/
  _siitcmram = LOADADDR(.itc_mram);

  /* The program routines to be located in ITCMRAM and executed from this memory */
  .itc_mram : {
  	. = ALIGN(4);
  	_sitcmram = .;        /* create a global symbol at data start */
  	*(.itc_mram)			  /* program routines to copy to ITCMRAM */
    . = ALIGN(4);
    _eitcmram = .;        /* define a global symbol at data end */
  } >ITCMRAM AT> OS_FLASH 


  /* Constant data into "OS_FLASH" memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >OS_FLASH

  .ARM.extab   : { 
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >OS_FLASH
  
  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >OS_FLASH

  .preinit_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >OS_FLASH
  
  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >OS_FLASH
  
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >OS_FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "OS_RAM" memory */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >OS_RAM AT> OS_FLASH


  /* Used by the startup to initialize copy routine*/
  _sidtcmram = LOADADDR(.dtc_mram);

  /* The program data to be located in DTCMRAM and accessed from this memory */
  .dtc_mram : {
  	. = ALIGN(4);
  	_sdtcmram = .;        /* create a global symbol at data start */
  	*(.dtc_mram)			  /* program routines to copy to DTCMRAM */
    . = ALIGN(4);
    _edtcmram = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> OS_FLASH 

  /* Uninitialized data section into "OS_RAM" memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >OS_RAM

  /* User_heap_stack section, used to check that there is enough "OS_RAM" memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >OS_RAM

  /* Uninitialized data section into "SH0_RAM" shared memory */
  .sh0_ram :
  {
  	. = ALIGN(4);
  	*(.sh0_ram)
  	. = ALIGN(4);
  } >SH0_RAM
  
  /* Uninitialized data section into "SH1_RAM" shared memory */
  .sh1_ram :
  {
  	. = ALIGN(4);
  	*(.sh1_ram)
  	. = ALIGN(4);
  } >SH1_RAM 
  
  /* Uninitialized data section into "BUF_RAM" shared memory */
  .buf_ram :
  {
  	. = ALIGN(4);
  	*(.buf_ram)
  	. = ALIGN(4);
  } >BUF_RAM 
  
  /* Uninitialized data section into "BK_RAM" backup memory */
  .bk_ram :
  {
    . = ALIGN(4);
  	*(.bk_ram)
  	. = ALIGN(4);
  } >BK_RAM
  
  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
