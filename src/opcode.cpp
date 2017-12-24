/*
** LHades
** Copyright (C) 2017 tacigar
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "config.hpp"
#include "opcode.hpp"

namespace {

static constexpr int SIZE_OP  = 6;
static constexpr int SIZE_A   = 8;
static constexpr int SIZE_B   = 9;
static constexpr int SIZE_C   = 9;
static constexpr int SIZE_Ax  = SIZE_A + SIZE_B + SIZE_C;
static constexpr int SIZE_Bx  = SIZE_B + SIZE_C;
static constexpr int POS_OP   = 0;
static constexpr int POS_A    = POS_OP + SIZE_OP;
static constexpr int POS_C    = POS_A + SIZE_A;
static constexpr int POS_B    = POS_C + SIZE_C;
static constexpr int POS_Bx   = POS_C;
static constexpr int POS_Ax   = POS_A;

auto genMask(int n) -> int
{ return ~((~(Instruction)0) << (n)); }

auto getOpCode(Instruction inst) -> int
{ return inst & genMask(SIZE_OP); }

auto getArgA(Instruction inst) -> int
{ return (inst >> POS_A) & genMask(SIZE_A); }

auto getArgB(Instruction inst) -> int
{ return (inst >> POS_B) & genMask(SIZE_B); }

auto getArgC(Instruction inst) -> int
{ return (inst >> POS_C) & genMask(SIZE_C); }

auto getArgAx(Instruction inst) -> int
{ return (inst >> POS_Ax) & genMask(SIZE_Ax); }

auto getArgBx(Instruction inst) -> int
{ return (inst >> POS_Bx) & genMask(SIZE_Bx); }

auto getArgSBx(Instruction inst) -> int
{ return (inst >> POS_Bx) & genMask(SIZE_Bx); }

auto genCodeAB(const std::string& op, Instruction inst) -> std::string
{
    std::stringstream ss;

    auto argA = getArgA(inst);
    auto argB = getArgB(inst);

    ss << std::left << std::setw(8) << op;
    ss << std::right << std::setw(7) << argA;
    ss << std::right << std::setw(7) << argB;

    return ss.str();
}

auto genCodeABC(const std::string& op, Instruction inst) -> std::string
{
    std::stringstream ss;

    auto argA = getArgA(inst);
    if (argA >= 256) argA -= 256;
    auto argB = getArgB(inst);
    if (argB >= 256) argB -= 256;
    auto argC = getArgC(inst);
    if (argC >= 256) argC -= 256;

    ss << std::left << std::setw(8) << op;
    ss << std::right << std::setw(7) << argA;
    ss << std::right << std::setw(7) << argB;
    ss << std::right << std::setw(7) << argC;

    return ss.str();
}

auto genCodeABx(const std::string& op, Instruction inst) -> std::string
{
    std::stringstream ss;
    auto argA = getArgA(inst);
    auto argBx = getArgBx(inst);

    ss << std::left << std::setw(8) << op;
    ss << std::right << std::setw(7) << argA;
    ss << std::right << std::setw(7) << argBx;

    return ss.str();
}

} // namespace

namespace OpCode {

auto genCode(Instruction inst) -> std::string
{
    switch (getOpCode(inst)) {
        case OP_MOVE: // 0x00
            return genCodeAB("MOVE", inst);
        case OP_LOADK: // 0x01
            return genCodeABx("LOADK", inst);
        case OP_LOADKX: // 0x02
        case OP_LOADBOOL: // 0x03
            return genCodeABC("LOADBOOL", inst);
        case OP_LOADNIL: // 0x04
            return genCodeAB("LOADNIL", inst);
        case OP_GETUPVAL: // 0x05
            return genCodeAB("GETUPVAL", inst);
        case OP_GETTABUP: // 0x06
            return genCodeABC("GETTABUP", inst);
        case OP_GETTABLE: // 0x07
            return genCodeABC("GETTABLE", inst);
        case OP_SETTABUP: // 0x08
            return genCodeABC("SETTABUP", inst);
        case OP_SETUPVAL: // 0x09
            return genCodeAB("SETUPVAL", inst);
        case OP_SETTABLE: // 0x0a
            return genCodeABC("SETTABLE", inst);
        case OP_NEWTABLE: // 0x0b
            return genCodeABC("NEWTABLE", inst);
        case OP_SELF: // 0x0c
            return genCodeABC("SELF", inst);
        case OP_ADD: // 0x0d
            return genCodeABC("ADD", inst);
        case OP_SUB: // 0x0e
            return genCodeABC("SUB", inst);
        case OP_MUL: // 0x0f
            return genCodeABC("MUL", inst);
        case OP_MOD: // 0x10
            return genCodeABC("MOD", inst);
        case OP_POW: // 0x11
            return genCodeABC("POW", inst);
        case OP_DIV: // 0x12
            return genCodeABC("DIV", inst);
        case OP_IDIV: // 0x13
            return genCodeABC("IDIV", inst);
        case OP_BAND: // 0x14
            return genCodeABC("BAND", inst);
        case OP_BOR: // 0x15
            return genCodeABC("BOR", inst);
        case OP_BXOR: // 0x16
            return genCodeABC("BXOR", inst);
        case OP_SHL: // 0x17
            return genCodeABC("SHL", inst);
        case OP_SHR: // 0x18
            return genCodeABC("SHR", inst);
        case OP_UNM: // 0x19
            return genCodeAB("UNM", inst);
        case OP_BNOT: // 0x1a
            return genCodeAB("BNOT", inst);
        case OP_NOT: // 0x1b
            return genCodeAB("NOT", inst);
        case OP_LEN: // 0x1c
            return genCodeAB("LEN", inst);
        case OP_CONCAT: // 0x1d
            return genCodeABC("CONCAT", inst);
        case OP_JMP: // 0x1e
        case OP_EQ: // 0x1f
            return genCodeABC("EQ", inst);
        case OP_LT: // 0x20
            return genCodeABC("LT", inst);
        case OP_LE: // 0x21
            return genCodeABC("LE", inst);
        case OP_TEST: // 0x22
        case OP_TESTSET: // 0x23
            return genCodeABC("TESTSET", inst);
        case OP_CALL: // 0x24
            return genCodeABC("CALL", inst);
        case OP_TAILCALL: // 0x25
            return genCodeABC("TAILCALL", inst);
        case OP_RETURN: // 0x26
            return genCodeAB("RETURN", inst);
        case OP_FORLOOP: // 0x27
        case OP_FORPREP: // 0x28
        case OP_TFORCALL: // 0x29
        case OP_TFORLOOP: // 0x2a
        case OP_SETLIST: // 0x2b
            return genCodeABC("SETLIST", inst);
        case OP_CLOSURE: // 0x2c
            return genCodeABx("CLOSURE", inst);
        case OP_VARARG: // 0x2d
            return genCodeAB("VARARG", inst);
        case OP_EXTRAARG: // 0x2e
            break;
    }
    return "";
}

} // namespace OpCode
