/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_CODE_HPP
#define LHADES_CODE_HPP

#include <limits>
#include <istream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <luaconf.h>

#include "config.hpp"

namespace lhades {

namespace detail {

    template <int SIZE_Bx, class = void>
    struct GenMAXARG_Bx;

    template <int SIZE_Bx>
    struct GenMAXARG_Bx <SIZE_Bx, typename std::enable_if<SIZE_Bx < LUAI_BITSINT - 1>::type>
    {
        static constexpr int MAXARG_Bx = (1 << SIZE_Bx) - 1;
        static constexpr int MAXARG_sBx = MAXARG_Bx >> 1;
    };

    template <int SIZE_Bx>
    struct GenMAXARG_Bx <SIZE_Bx, typename std::enable_if<SIZE_Bx >= LUAI_BITSINT - 1>::type>
    {
        static constexpr int MAXARG_Bx = std::numeric_limits<int>::max();
        static constexpr int MAXARG_sBx = std::numeric_limits<int>::max();
    };

    template <int SIZE_Ax, class = void>
    struct GenMAXARG_Ax;

    template <int SIZE_Ax>
    struct GenMAXARG_Ax <SIZE_Ax, typename std::enable_if<SIZE_Ax < LUAI_BITSINT - 1>::type>
    {
        static constexpr int MAXARG_Ax = (1 << SIZE_Ax) - 1;
    };

    template <int SIZE_Ax>
    struct GenMAXARG_Ax <SIZE_Ax, typename std::enable_if<SIZE_Ax >= LUAI_BITSINT - 1>::type>
    {
        static constexpr int MAXARG_Ax = std::numeric_limits<int>::max();
    };

} // namespace detail

class Code
{
public:
    enum OpCode
    {
        OP_MOVE      = 0x00, // A B    R(A) := R(B)
        OP_LOADK     = 0x01, // A Bx   R(A) := Kst(Bx)
        OP_LOADKX    = 0x02, // A      R(A) := Kst(extra arg)
        OP_LOADBOOL  = 0x03, // A B C  R(A) := (Bool)B; if (C) pc++
        OP_LOADNIL   = 0x04, // A B    R(A), R(A+1), ..., R(A+B) := nil
        OP_GETUPVAL  = 0x05, // A B    R(A) := UpValue[B]
        OP_GETTABUP  = 0x06, // A B C  R(A) := UpValue[B][RK(C)]
        OP_GETTABLE  = 0x07, // A B C  R(A) := R(B)[RK(C)]
        OP_SETTABUP  = 0x08, // A B C  UpValue[A][RK(B)] := RK(C)
        OP_SETUPVAL  = 0x09, // A B    UpValue[B] := R(A)
        OP_SETTABLE  = 0x0a, // A B C  R(A)[RK(B)] := RK(C)
        OP_NEWTABLE  = 0x0b, // A B C  R(A) := {} (size = B,C)
        OP_SELF      = 0x0c, // A B C  R(A+1) := R(B); R(A) := R(B)[RK(C)]
        OP_ADD       = 0x0d, // A B C  R(A) := RK(B) + RK(C)
        OP_SUB       = 0x0e, // A B C  R(A) := RK(B) - RK(C)
        OP_MUL       = 0x0f, // A B C  R(A) := RK(B) * RK(C)
        OP_MOD       = 0x10, // A B C  R(A) := RK(B) % RK(C)
        OP_POW       = 0x11, // A B C  R(A) := RK(B) ^ RK(C)
        OP_DIV       = 0x12, // A B C  R(A) := RK(B) / RK(C)
        OP_IDIV      = 0x13, // A B C  R(A) := RK(B) // RK(C)
        OP_BAND      = 0x14, // A B C  R(A) := RK(B) & RK(C)
        OP_BOR       = 0x15, // A B C  R(A) := RK(B) | RK(C)
        OP_BXOR      = 0x16, // A B C  R(A) := RK(B) ~ RK(C)
        OP_SHL       = 0x17, // A B C  R(A) := RK(B) << RK(C)
        OP_SHR       = 0x18, // A B C  R(A) := RK(B) >> RK(C)
        OP_UNM       = 0x19, // A B    R(A) := -R(B)
        OP_BNOT      = 0x1a, // A B    R(A) := ~R(B)
        OP_NOT       = 0x1b, // A B    R(A) := not R(B)
        OP_LEN       = 0x1c, // A B    R(A) := length of R(B)
        OP_CONCAT    = 0x1d, // A B C  R(A) := R(B).. ... ..R(C)
        OP_JMP       = 0x1e, // A sBx  pc+=sBx; if (A) close all upvalues >= R(A - 1)
        OP_EQ        = 0x1f, // A B C  if ((RK(B) == RK(C)) ~= A) then pc++
        OP_LT        = 0x20, // A B C  if ((RK(B) <  RK(C)) ~= A) then pc++
        OP_LE        = 0x21, // A B C  if ((RK(B) <= RK(C)) ~= A) then pc++
        OP_TEST      = 0x22, // A C    if not (R(A) <=> C) then pc++
        OP_TESTSET   = 0x23, // A B C  if (R(B) <=> C) then R(A) := R(B) else pc++
        OP_CALL      = 0x24, // A B C  R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
        OP_TAILCALL  = 0x25, // A B C  return R(A)(R(A+1), ... ,R(A+B-1))
        OP_RETURN    = 0x26, // A B    return R(A), ... ,R(A+B-2)
        OP_FORLOOP   = 0x27, // A sBx  R(A)+=R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }
        OP_FORPREP   = 0x28, // A sBx  R(A)-=R(A+2); pc+=sBx
        OP_TFORCALL  = 0x29, // A C    R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
        OP_TFORLOOP  = 0x2a, // A sBx  if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }
        OP_SETLIST   = 0x2b, // A B C  R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
        OP_CLOSURE   = 0x2c, // A Bx   R(A) := closure(KPROTO[Bx])
        OP_VARARG    = 0x2d, // A B    R(A), R(A+1), ..., R(A+B-2) = vararg
        OP_EXTRAARG  = 0x2e  // Ax     extra (larger) argument for previous opcode
    };

    enum OpCodeType
    {
        TYPE_AB    = 0x00,
        TYPE_AC    = 0x01,
        TYPE_ABC   = 0x02,
        TYPE_ABx   = 0x03,
        TYPE_ASBx  = 0x04,
    };

    static auto read(std::istream &is) -> std::shared_ptr<Code>;
    static auto getOpCodeType(OpCode o) -> OpCodeType;
    static auto opCodeToString(OpCode o) -> std::string;
    static const std::unordered_map<OpCode, std::string> COMMENTS;

    Code();
    Code(const Code& other);
    auto operator=(const Code& other) -> const Code&;
    ~Code() = default;

    auto opCode() const -> OpCode { return m_opCode; }
    auto A() const -> int { return m_A; }
    auto B() const -> int { return m_B; }
    auto C() const -> int { return m_C; }
    auto Ax() const -> int { return m_Ax; }
    auto Bx() const -> int { return m_Bx; }
    auto sBx() const -> int { return m_sBx; }
    auto comment() const -> std::string;

private:
    static constexpr int SIZE_OP = 6;
    static constexpr int SIZE_A  = 8;
    static constexpr int SIZE_B  = 9;
    static constexpr int SIZE_C  = 9;
    static constexpr int SIZE_Ax = SIZE_A + SIZE_B + SIZE_C;
    static constexpr int SIZE_Bx = SIZE_B + SIZE_C;
    static constexpr int POS_OP  = 0;
    static constexpr int POS_A   = POS_OP + SIZE_OP;
    static constexpr int POS_C   = POS_A + SIZE_A;
    static constexpr int POS_B   = POS_C + SIZE_C;
    static constexpr int POS_Bx  = POS_C;
    static constexpr int POS_Ax  = POS_A;

    static constexpr int MAXARG_Bx  = detail::GenMAXARG_Bx<SIZE_Bx>::MAXARG_Bx;
    static constexpr int MAXARG_sBx = detail::GenMAXARG_Bx<SIZE_Bx>::MAXARG_sBx;
    static constexpr int MAXARG_Ax  = detail::GenMAXARG_Ax<SIZE_Ax>::MAXARG_Ax;

    static auto getMask(int n) -> int { return ~((~(Instruction)0) << (n)); }
    static auto getOpCode(Instruction inst) -> int { return inst & getMask(SIZE_OP); }
    static auto getArgA(Instruction inst) -> int { return (inst >> POS_A) & getMask(SIZE_A); }
    static auto getArgB(Instruction inst) -> int { return (inst >> POS_B) & getMask(SIZE_B); }
    static auto getArgC(Instruction inst) -> int { return (inst >> POS_C) & getMask(SIZE_C); }
    static auto getArgAx(Instruction inst) -> int { return (inst >> POS_Ax) & getMask(SIZE_Ax); }
    static auto getArgBx(Instruction inst) -> int { return (inst >> POS_Bx) & getMask(SIZE_Bx); }
    static auto getArgSBx(Instruction inst) -> int { return getArgBx(inst) - MAXARG_sBx; }

private:
    OpCode m_opCode;
    int m_A;
    int m_B;
    int m_C;
    int m_Ax;
    int m_Bx;
    int m_sBx;
};

} // namespace lhades

#endif // LHADES_CODE_HPP
