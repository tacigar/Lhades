/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <exception>
#include <istream>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>

#include "code.hpp"
#include "reader.hpp"

namespace lhades {

const std::unordered_map<Code::OpCode, std::string> Code::COMMENTS = {
    { Code::OP_MOVE     , "R(A) := R(B)" },
    { Code::OP_LOADK    , "R(A) := Kst(Bx)" },
    { Code::OP_LOADKX   , "R(A) := Kst(extra arg)" },
    { Code::OP_LOADBOOL , "R(A) := (Bool)B; if (C) pc++" },
    { Code::OP_LOADNIL  , "R(A), R(A+1), ..., R(A+B) := nil" },
    { Code::OP_GETUPVAL , "R(A) := UpValue[B]" },
    { Code::OP_GETTABUP , "R(A) := UpValue[B][RK(C)]" },
    { Code::OP_GETTABLE , "R(A) := R(B)[RK(C)]" },
    { Code::OP_SETTABUP , "UpValue[A][RK(B)] := RK(C)" },
    { Code::OP_SETUPVAL , "UpValue[B] := R(A)" },
    { Code::OP_SETTABLE , "R(A)[RK(B)] := RK(C)" },
    { Code::OP_NEWTABLE , "R(A) := {} (size = B,C)" },
    { Code::OP_SELF     , "R(A+1) := R(B); R(A) := R(B)[RK(C)]" },
    { Code::OP_ADD      , "R(A) := RK(B) + RK(C)" },
    { Code::OP_SUB      , "R(A) := RK(B) - RK(C)" },
    { Code::OP_MUL      , "R(A) := RK(B) * RK(C)" },
    { Code::OP_MOD      , "R(A) := RK(B) % RK(C)" },
    { Code::OP_POW      , "R(A) := RK(B) ^ RK(C)" },
    { Code::OP_DIV      , "R(A) := RK(B) / RK(C)" },
    { Code::OP_IDIV     , "R(A) := RK(B) // RK(C)" },
    { Code::OP_BAND     , "R(A) := RK(B) & RK(C)" },
    { Code::OP_BOR      , "R(A) := RK(B) | RK(C)" },
    { Code::OP_BXOR     , "R(A) := RK(B) ~ RK(C)" },
    { Code::OP_SHL      , "R(A) := RK(B) << RK(C)" },
    { Code::OP_SHR      , "R(A) := RK(B) >> RK(C)" },
    { Code::OP_UNM      , "R(A) := -R(B)" },
    { Code::OP_BNOT     , "R(A) := ~R(B)" },
    { Code::OP_NOT      , "R(A) := not R(B)" },
    { Code::OP_LEN      , "R(A) := length of R(B)" },
    { Code::OP_CONCAT   , "R(A) := R(B).. ... ..R(C)" },
    { Code::OP_JMP      , "pc+=sBx; if (A) close all upvalues >= R(A - 1)" },
    { Code::OP_EQ       , "if ((RK(B) == RK(C)) ~= A) then pc++" },
    { Code::OP_LT       , "if ((RK(B) <  RK(C)) ~= A) then pc++" },
    { Code::OP_LE       , "if ((RK(B) <= RK(C)) ~= A) then pc++" },
    { Code::OP_TEST     , "if not (R(A) <=> C) then pc++" },
    { Code::OP_TESTSET  , "if (R(B) <=> C) then R(A) := R(B) else pc++" },
    { Code::OP_CALL     , "R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))" },
    { Code::OP_TAILCALL , "return R(A)(R(A+1), ... ,R(A+B-1))" },
    { Code::OP_RETURN   , "return R(A), ... ,R(A+B-2)" },
    { Code::OP_FORLOOP  , "R(A)+=R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }" },
    { Code::OP_FORPREP  , "R(A)-=R(A+2); pc+=sBx" },
    { Code::OP_TFORCALL , "R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));" },
    { Code::OP_TFORLOOP , "if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }" },
    { Code::OP_SETLIST  , "R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B" },
    { Code::OP_CLOSURE  , "R(A) := closure(KPROTO[Bx])" },
    { Code::OP_VARARG   , "R(A), R(A+1), ..., R(A+B-2) = vararg" },
    { Code::OP_EXTRAARG , "extra (larger) argument for previous opcode" },
};

Code::Code()
{
}

Code::Code(const Code& other)
    : m_opCode(other.m_opCode)
    , m_A(other.m_A)
    , m_B(other.m_B)
    , m_C(other.m_C)
    , m_Ax(other.m_Ax)
    , m_Bx(other.m_Bx)
    , m_sBx(other.m_sBx)
{
}

auto Code::operator=(const Code& other) -> const Code&
{
    m_opCode = other.m_opCode;
    m_A = other.m_A;
    m_B = other.m_B;
    m_C = other.m_C;
    m_Ax = other.m_Ax;
    m_Bx = other.m_Bx;
    m_sBx = other.m_sBx;

    return *this;
}

auto Code::read(std::istream &is) -> std::shared_ptr<Code>
{
    auto code = std::make_shared<Code>();
    auto inst = Reader::read<Instruction>(is);
    auto opCode = getOpCode(inst);
    code->m_opCode = static_cast<OpCode>(opCode);

    switch (getOpCodeType(code->m_opCode)) {
        case TYPE_AB:
            code->m_A = getArgA(inst);
            code->m_B = getArgB(inst);
            break;
        case TYPE_AC:
            code->m_A = getArgA(inst);
            code->m_C = getArgC(inst);
            break;
        case TYPE_ABC:
            code->m_A = getArgA(inst);
            code->m_B = getArgB(inst);
            code->m_C = getArgC(inst);
            break;
        case TYPE_ABx:
            code->m_A = getArgA(inst);
            code->m_Bx = getArgBx(inst);
            break;
        case TYPE_ASBx:
            code->m_A = getArgA(inst);
            code->m_sBx = getArgSBx(inst);
            break;
    }
    if (code->m_A >= 256) code->m_A -= 256;
    if (code->m_B >= 256) code->m_B -= 256;
    if (code->m_C >= 256) code->m_C -= 256;

    return code;
}

auto Code::getOpCodeType(OpCode o) -> OpCodeType
{
    switch (o) {
        case OP_MOVE:     return TYPE_AB;
        case OP_LOADK:    return TYPE_ABx;
        // case OP_LOADKX:
        case OP_LOADBOOL: return TYPE_ABC;
        case OP_LOADNIL:  return TYPE_AB;
        case OP_GETUPVAL: return TYPE_AB;
        case OP_GETTABUP: return TYPE_ABC;
        case OP_GETTABLE: return TYPE_ABC;
        case OP_SETTABUP: return TYPE_ABC;
        case OP_SETUPVAL: return TYPE_AB;
        case OP_SETTABLE: return TYPE_ABC;
        case OP_NEWTABLE: return TYPE_ABC;
        case OP_SELF:     return TYPE_ABC;
        case OP_ADD:      return TYPE_ABC;
        case OP_SUB:      return TYPE_ABC;
        case OP_MUL:      return TYPE_ABC;
        case OP_MOD:      return TYPE_ABC;
        case OP_POW:      return TYPE_ABC;
        case OP_DIV:      return TYPE_ABC;
        case OP_IDIV:     return TYPE_ABC;
        case OP_BAND:     return TYPE_ABC;
        case OP_BOR:      return TYPE_ABC;
        case OP_BXOR:     return TYPE_ABC;
        case OP_SHL:      return TYPE_ABC;
        case OP_SHR:      return TYPE_ABC;
        case OP_UNM:      return TYPE_AB;
        case OP_BNOT:     return TYPE_AB;
        case OP_NOT:      return TYPE_AB;
        case OP_LEN:      return TYPE_AB;
        case OP_CONCAT:   return TYPE_ABC;
        case OP_JMP:      return TYPE_ASBx;
        case OP_EQ:       return TYPE_ABC;
        case OP_LT:       return TYPE_ABC;
        case OP_LE:       return TYPE_ABC;
        case OP_TEST:     return TYPE_AC;
        case OP_TESTSET:  return TYPE_ABC;
        case OP_CALL:     return TYPE_ABC;
        case OP_TAILCALL: return TYPE_ABC;
        case OP_RETURN:   return TYPE_AB;
        case OP_FORLOOP:  return TYPE_ASBx;
        case OP_FORPREP:  return TYPE_ASBx;
        case OP_TFORCALL: return TYPE_AC;
        case OP_TFORLOOP: return TYPE_ASBx;
        case OP_SETLIST:  return TYPE_ABC;
        case OP_CLOSURE:  return TYPE_ABx;
        case OP_VARARG:   return TYPE_AB;
        //case OP_EXTRAARG:
        default:
            throw std::runtime_error("invalid opcode");
            break;
    }
}

auto Code::opCodeToString(OpCode o) -> std::string
{
    switch (o) {
        case OP_MOVE:     return "MOVE";
        case OP_LOADK:    return "LOADK";
        // case OP_LOADKX:
        case OP_LOADBOOL: return "LOADBOOL";
        case OP_LOADNIL:  return "NIL";
        case OP_GETUPVAL: return "GETUPVAL";
        case OP_GETTABUP: return "GETTABUP";
        case OP_GETTABLE: return "GETTABLE";
        case OP_SETTABUP: return "SETTABUP";
        case OP_SETUPVAL: return "SETUPVAL";
        case OP_SETTABLE: return "SETTABLE";
        case OP_NEWTABLE: return "NEWTABLE";
        case OP_SELF:     return "SELF";
        case OP_ADD:      return "ADD";
        case OP_SUB:      return "SUB";
        case OP_MUL:      return "MUL";
        case OP_MOD:      return "MOD";
        case OP_POW:      return "POW";
        case OP_DIV:      return "DIV";
        case OP_IDIV:     return "IDIV";
        case OP_BAND:     return "BAND";
        case OP_BOR:      return "BOR";
        case OP_BXOR:     return "BXOR";
        case OP_SHL:      return "SHL";
        case OP_SHR:      return "SHR";
        case OP_UNM:      return "UNM";
        case OP_BNOT:     return "BNOT";
        case OP_NOT:      return "NOT";
        case OP_LEN:      return "LEN";
        case OP_CONCAT:   return "CONCAT";
        case OP_JMP:      return "JMP";
        case OP_EQ:       return "EQ";
        case OP_LT:       return "LT";
        case OP_LE:       return "LE";
        case OP_TEST:     return "TEST";
        case OP_TESTSET:  return "TESTSET";
        case OP_CALL:     return "CALL";
        case OP_TAILCALL: return "TAILCALL";
        case OP_RETURN:   return "RETURN";
        case OP_FORLOOP:  return "FORLOOP";
        case OP_FORPREP:  return "FORPREP";
        case OP_TFORCALL: return "TFORCALL";
        case OP_TFORLOOP: return "TFORLOOP";
        case OP_SETLIST:  return "SETLIST";
        case OP_CLOSURE:  return "CLOSURE";
        case OP_VARARG:   return "VARARG";
        //case OP_EXTRAARG:
        default:
            throw std::runtime_error("invalid opcode");
            break;
    }
}

auto Code::comment() const -> std::string
{
    auto str = COMMENTS.at(m_opCode);
    str = std::regex_replace(str, std::regex("sBx"), std::to_string(m_sBx));
    str = std::regex_replace(str, std::regex("Ax"), std::to_string(m_Ax));
    str = std::regex_replace(str, std::regex("Bx"), std::to_string(m_Bx));
    str = std::regex_replace(str, std::regex("A"), std::to_string(m_A));
    str = std::regex_replace(str, std::regex("B"), std::to_string(m_B));
    str = std::regex_replace(str, std::regex("C"), std::to_string(m_C));
    return str;
}

} // namespace lhades
