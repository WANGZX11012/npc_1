// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

extern "C" void npc_ebreak(int code);

void Vtop___024unit____Vdpiimwrap_npc_ebreak_TOP____024unit(IData/*31:0*/ code) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_npc_ebreak_TOP____024unit\n"); );
    // Body
    int code__Vcvt;
    code__Vcvt = code;
    npc_ebreak(code__Vcvt);
}

extern "C" void pmem_write_u32(int waddr, int wdata);

void Vtop___024unit____Vdpiimwrap_pmem_write_u32_TOP____024unit(IData/*31:0*/ waddr, IData/*31:0*/ wdata) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_pmem_write_u32_TOP____024unit\n"); );
    // Body
    int waddr__Vcvt;
    waddr__Vcvt = waddr;
    int wdata__Vcvt;
    wdata__Vcvt = wdata;
    pmem_write_u32(waddr__Vcvt, wdata__Vcvt);
}

extern "C" void pmem_write_u8(int waddr, char wdata);

void Vtop___024unit____Vdpiimwrap_pmem_write_u8_TOP____024unit(IData/*31:0*/ waddr, CData/*7:0*/ wdata) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_pmem_write_u8_TOP____024unit\n"); );
    // Body
    int waddr__Vcvt;
    waddr__Vcvt = waddr;
    char wdata__Vcvt;
    wdata__Vcvt = wdata;
    pmem_write_u8(waddr__Vcvt, wdata__Vcvt);
}
