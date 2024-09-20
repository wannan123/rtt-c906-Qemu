/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020/12/12     bernard      The first version
 */

INCLUDE "link_stacksize.lds"

OUTPUT_ARCH( "riscv" )

/*
 * Memory layout:
 * 0x50000000 - 0x50200000: SBI
 * 0x50200000 - 0x51200000: Kernel
 */


MEMORY
{
	DRAM   : ORIGIN = 0x50000000, LENGTH = 0x100000   /* on-chip DRAM 1*1MB */
}

__min_heap_size = 0x200;
PROVIDE (__ram_end  = 0x50100000 - 0x8);
PROVIDE (__heap_end = __ram_end);

ENTRY(Reset_Handler)
SECTIONS
{
.text : {
   KEEP (*(.vectors))
   *(.boot)
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t.*)
   *(.plt)
   *(.gnu_warning)
   *(.gcc_execpt_table)
   *(.glue_7)
   *(.glue_7t)
   *(.ARM.extab)
   *(.gnu.linkonce.armextab.*)

   /* section information for finsh shell */
   . = ALIGN(8);
   __fsymtab_start = .;
   KEEP(*(FSymTab))
   __fsymtab_end = .;
   . = ALIGN(8);
   __vsymtab_start = .;
   KEEP(*(VSymTab))
   __vsymtab_end = .;
   . = ALIGN(8);

   /* section information for initial. */
   . = ALIGN(8);
   __rt_init_start = .;
   KEEP(*(SORT(.rti_fn*)))
   __rt_init_end = .;
   . = ALIGN(8);

   __rt_utest_tc_tab_start = .;
   KEEP(*(UtestTcTab))
   __rt_utest_tc_tab_end = .;
} > DRAM

.init (ALIGN(64)) : {
   KEEP (*(.init))
} > DRAM

.fini (ALIGN(64)) : {
   KEEP (*(.fini))
} > DRAM

.interp : {
   KEEP (*(.interp))
} > DRAM

.note-ABI-tag : {
   KEEP (*(.note-ABI-tag))
} > DRAM

.rodata : {
   . = ALIGN(64);
   __rodata_start = .;
   *(.rodata)
   *(.rodata.*)
   *(.srodata*)
   *(.gnu.linkonce.r.*)
   __rodata_end = .;
} > DRAM

.rodata1 : {
   . = ALIGN(64);
   __rodata1_start = .;
   *(.rodata1)
   *(.rodata1.*)
   __rodata1_end = .;
} > DRAM
.data : {
  . = ALIGN(0x4) ;
  __sdata = . ;
  __data_start__ = . ;
  data_start = . ;
  *(.got.plt)
  *(.got)
  *(.gnu.linkonce.r*)
  *(.data)
  *(.data*)
  *(.data1)
  *(.data.*)
  *(.gnu.linkonce.d*)
  *(.data1)
  *(.gcc_except_table)
  *(.gcc_except_table*)
  __start_init_call = .;
  *(.initcall.init)
  __stop_init_call = .;
  __start_cmd = .;
  *(.bootloaddata.cmd)
  . = ALIGN(4) ;
  __stop_cmd = .;
  __global_pointer$ = .;
  *(.sdata)
  *(.sdata.*)
  *(.sdata2.*)
  *(.gnu.linkonce.s.*)
  *(__libc_atexit)
  *(__libc_subinit)
  *(__libc_subfreeres)
  *(.note.ABI-tag)
  __edata = .;
  __data_end__ = .;
  . = ALIGN(0x4) ;
} > DRAM


.data1 : {
   . = ALIGN(64);
   __data1_start = .;
   *(.data1)
   *(.data1.*)
   __data1_end = .;
} > DRAM

.got : {
   *(.got)
} > DRAM

.got1 : {
   *(.got1)
} > DRAM

.got2 : {
   *(.got2)
} > DRAM

.ctors : {
   . = ALIGN(64);
   __CTOR_LIST__ = .;
   ___CTORS_LIST___ = .;
   KEEP (*crtbegin.o(.ctors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .ctors))
   KEEP (*(SORT(.ctors.*)))
   KEEP (*(.ctors))
   __CTOR_END__ = .;
   ___CTORS_END___ = .;
} > DRAM

.dtors : {
   . = ALIGN(64);
   __DTOR_LIST__ = .;
   ___DTORS_LIST___ = .;
   KEEP (*crtbegin.o(.dtors))
   KEEP (*(EXCLUDE_FILE(*crtend.o) .dtors))
   KEEP (*(SORT(.dtors.*)))
   KEEP (*(.dtors))
   __DTOR_END__ = .;
   ___DTORS_END___ = .;
} > DRAM

.fixup : {
   __fixup_start = .;
   *(.fixup)
   __fixup_end = .;
} > DRAM

.eh_frame : {
   *(.eh_frame)
} > DRAM

.eh_framehdr : {
   __eh_framehdr_start = .;
   *(.eh_framehdr)
   __eh_framehdr_end = .;
} > DRAM

.gcc_except_table : {
   *(.gcc_except_table)
} > DRAM

 .bss : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_start__ = . ;
  *(.dynsbss)
  *(.sbss)
  *(.sbss.*)
  *(.scommon)
  *(.dynbss)
  *(.bss)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __bss_end__ = .;
  __end = . ;
  end = . ;
 } > DRAM

/*_SDA_BASE_ = __sdata_start + ((__sbss_end - __sdata_start) / 2 );*/
    _data_lma = LOADADDR(.data);

/* Generate Stack and Heap definitions */
.stack (NOLOAD) : {
   . = ALIGN(64);
   _stack_end_end = .;
   . += _STACK_SIZE;
   _stack_top = .;
  __rt_rvstack = .; 
} > DRAM

 ._user_heap (NOLOAD): {
  . = ALIGN(0x4) ;
  *(.stack*)
  . = ALIGN(0x4) ;
  __heap_start = .;
  . += __min_heap_size;
  . = ALIGN(0x4) ;
 } > DRAM

HeapLimit = ORIGIN(DRAM) + LENGTH(DRAM);
_end = .;
}
