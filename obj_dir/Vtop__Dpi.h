// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#ifndef VERILATED_VTOP__DPI_H_
#define VERILATED_VTOP__DPI_H_  // guard

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI IMPORTS
    // DPI import at vsrc/dpic_ebreak.v:1:30
    extern void npc_ebreak(int code);
    // DPI import at vsrc/LSU.v:2:30
    extern void pmem_write_u32(int waddr, int wdata);
    // DPI import at vsrc/LSU.v:3:30
    extern void pmem_write_u8(int waddr, char wdata);

#ifdef __cplusplus
}
#endif

#endif  // guard
