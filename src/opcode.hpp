/*
** LHades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_OPCODE_HPP
#define LHADES_OPCODE_HPP

#include <string>

#include "config.hpp"

namespace OpCode {

enum {
    OP_MOVE     = 0x00, // A B R(A) := R(B)
    OP_LOADK    = 0x01, // A Bx R(A) := Kst(Bx)
    OP_LOADKX   = 0x02, // A R(A) := Kst(extra arg)
    OP_LOADBOOL = 0x03, // A B C R(A) := (Bool)B; if (C) pc++
    OP_LOADNIL  = 0x04, // A B R(A), R(A+1), ..., R(A+B) := nil
    OP_GETUPVAL = 0x05, /*	A B	R(A) := UpValue[B]				*/
    OP_GETTABUP = 0x06,/*	A B C	R(A) := UpValue[B][RK(C)]			*/
    OP_GETTABLE = 0x07,/*	A B C	R(A) := R(B)[RK(C)]				*/
    OP_SETTABUP = 0x08,/*	A B C	UpValue[A][RK(B)] := RK(C)			*/
    OP_SETUPVAL = 0x09,/*	A B	UpValue[B] := R(A)				*/
    OP_SETTABLE = 0x0a,/*	A B C	R(A)[RK(B)] := RK(C)				*/
    OP_NEWTABLE = 0x0b,/*	A B C	R(A) := {} (size = B,C)				*/
    OP_SELF     = 0x0c,/*	A B C	R(A+1) := R(B); R(A) := R(B)[RK(C)]		*/
    OP_ADD      = 0x0d,/*	A B C	R(A) := RK(B) + RK(C)				*/
    OP_SUB      = 0x0e,/*	A B C	R(A) := RK(B) - RK(C)				*/
    OP_MUL      = 0x0f,/*	A B C	R(A) := RK(B) * RK(C)				*/
    OP_MOD      = 0x10,/*	A B C	R(A) := RK(B) % RK(C)				*/
    OP_POW      = 0x11,/*	A B C	R(A) := RK(B) ^ RK(C)				*/
    OP_DIV      = 0x12,/*	A B C	R(A) := RK(B) / RK(C)				*/
    OP_IDIV     = 0x13,/*	A B C	R(A) := RK(B) // RK(C)				*/
    OP_BAND     = 0x14,/*	A B C	R(A) := RK(B) & RK(C)				*/
    OP_BOR      = 0x15,/*	A B C	R(A) := RK(B) | RK(C)				*/
    OP_BXOR     = 0x16,/*	A B C	R(A) := RK(B) ~ RK(C)				*/
    OP_SHL      = 0x17,/*	A B C	R(A) := RK(B) << RK(C)				*/
    OP_SHR      = 0x18,/*	A B C	R(A) := RK(B) >> RK(C)				*/
    OP_UNM      = 0x19,/*	A B	R(A) := -R(B)					*/
    OP_BNOT     = 0x1a,/*	A B	R(A) := ~R(B)					*/
    OP_NOT      = 0x1b,/*	A B	R(A) := not R(B)				*/
    OP_LEN      = 0x1c,/*	A B	R(A) := length of R(B)				*/
    OP_CONCAT   = 0x1d,/*	A B C	R(A) := R(B).. ... ..R(C)			*/
    OP_JMP      = 0x1e,/*	A sBx	pc+=sBx; if (A) close all upvalues >= R(A - 1)	*/
    OP_EQ       = 0x1f,/*	A B C	if ((RK(B) == RK(C)) ~= A) then pc++		*/
    OP_LT       = 0x20,/*	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++		*/
    OP_LE       = 0x21,/*	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++		*/
    OP_TEST     = 0x22,/*	A C	if not (R(A) <=> C) then pc++			*/
    OP_TESTSET  = 0x23,/*	A B C	if (R(B) <=> C) then R(A) := R(B) else pc++	*/
    OP_CALL     = 0x24, // A B C  R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
    OP_TAILCALL = 0x25,/*	A B C	return R(A)(R(A+1), ... ,R(A+B-1))		*/
    OP_RETURN   = 0x26, // A B  return R(A), ... ,R(A+B-2)
    OP_FORLOOP  = 0x27,/*	A sBx	R(A)+=R(A+2);
        if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }*/
    OP_FORPREP  = 0x28,/*	A sBx	R(A)-=R(A+2); pc+=sBx				*/
    OP_TFORCALL = 0x29,/*	A C	R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));	*/
    OP_TFORLOOP = 0x2a,/*	A sBx	if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }*/
    OP_SETLIST  = 0x2b,/*	A B C	R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B	*/
    OP_CLOSURE  = 0x2c, // A Bx  R(A) := closure(KPROTO[Bx])
    OP_VARARG   = 0x2d,/*	A B	R(A), R(A+1), ..., R(A+B-2) = vararg		*/
    OP_EXTRAARG = 0x2e/*	Ax	extra (larger) argument for previous opcode	*/
};

auto genCode(Instruction inst) -> std::string;

}

#endif // LHADES_OPCODE_HPP
