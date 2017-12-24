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
        case OP_LOADK: // 0x01
            return genCodeABx("LOADK", inst);
        case OP_GETTABUP: // 0x06
            return genCodeABC("GETTABUP", inst);
        case OP_SETTABUP: // 0x08
            return genCodeABC("SETTABUP", inst);
        case OP_ADD: // 0x0d
            return genCodeABC("ADD", inst);
        case OP_CALL: // 0x24
            return genCodeABC("CALL", inst);
        case OP_RETURN: // 0x26
            return genCodeAB("RETURN", inst);
        case OP_CLOSURE: // 0x2c
            return genCodeABx("CLOSURE", inst);
    }
    return "";
}

} // namespace OpCode
