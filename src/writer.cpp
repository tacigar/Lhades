/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <lua.h>

#include "code.hpp"
#include "constant.hpp"
#include "debug_info.hpp"
#include "proto.hpp"
#include "upvalue.hpp"
#include "writer.hpp"

namespace lhades {

auto Writer::write(std::ostream& os, const std::shared_ptr<Proto> proto) -> void
{
    writeProto(0, os, proto);
}

auto Writer::writeSpace(int level, std::ostream& os) -> void
{
    for (int i = 0; i < level; ++i) {
        os << "    ";
    }
}

auto Writer::writeProto(int level, std::ostream& os, const std::shared_ptr<Proto> proto) -> void
{
    os << "\x1b[39m";
    os << "source name \"" << proto->source() << "\": "
       << proto->lineDefined() << " - " << proto->lastLineDefined() << ": "
       << proto->numParams() << " params, "
       << proto->maxStackSize() << " stacks";

    auto digit = 0;

    auto codes = proto->codes();
    os << std::endl;  writeSpace(level, os);
    os << ".codes: " << codes.size();
    digit = std::floor(std::log10(codes.size())) + 1;
    for (int i = 0; i < codes.size(); ++i) {
        std::stringstream ss;
        ss << std::right << std::setw(digit) << i + 1;
        os << std::endl;  writeSpace(level, os);
        os << "  \x1b[32m[" << ss.str() << "] ";
        writeCode(os, codes[i]);
    }

    auto constants = proto->constants();
    os << std::endl;  writeSpace(level, os);
    os << ".consts: " << constants.size();
    digit = std::floor(std::log10(constants.size())) + 1;
    for (int i = 0; i < constants.size(); ++i) {
        std::stringstream ss;
        ss << std::right << std::setw(digit) << i;
        os << std::endl;  writeSpace(level, os);
        os << "  \x1b[32m[" << ss.str() << "] ";
        writeConstant(os, constants[i]);
    }

    auto upValues = proto->upValues();
    auto debugInfo = proto->debugInfo();
    os << std::endl;  writeSpace(level, os);
    os << ".upvalues: " << upValues.size();
    digit = std::floor(std::log10(upValues.size())) + 1;
    for (int i = 0; i < upValues.size(); ++i) {
        std::stringstream ss;
        ss << std::right << std::setw(digit) << i;
        os << std::endl;  writeSpace(level, os);
        os << "  \x1b[32m[" << ss.str() << "] ";
        writeUpValue(os, upValues[i], proto->debugInfo()->upValues()[i]);
    }

    auto localVars = proto->debugInfo()->localVars();
    os << std::endl;  writeSpace(level, os);
    os << ".locals: " << localVars.size();
    digit = std::floor(std::log10(localVars.size())) + 1;
    for (int i = 0; i < localVars.size(); ++i) {
        std::stringstream ss;
        ss << std::right << std::setw(digit) << i;
        os << std::endl;  writeSpace(level, os);
        os << "  \x1b[32m[" << ss.str() << "] ";
        writeLocalVar(os, localVars[i]);
    }

    auto protos = proto->protos();
    os << std::endl;  writeSpace(level, os);
    os << ".protos: " << protos.size();
    digit = std::floor(std::log10(protos.size())) + 1;
    for (int i = 0; i < protos.size(); ++i) {
        std::stringstream ss;
        ss << std::right << std::setw(digit) << i;
        os << std::endl;  writeSpace(level, os);
        os << "  \x1b[32m[" << ss.str() << "] ";
        writeProto(level + 1, os, protos[i]);
    }
}

auto Writer::writeCode(std::ostream& os, const std::shared_ptr<Code> code) -> void
{
    os << "\x1b[33m" << std::left << std::setw(9)
       << Code::opCodeToString(code->opCode());
    os << "\x1b[39m";

    auto type = Code::getOpCodeType(code->opCode());
    switch (type) {
        case Code::TYPE_AB:
            os << std::right << std::setw(8) << code->A();
            os << "        "; // for align
            os << std::right << std::setw(8) << code->B();
            break;
        case Code::TYPE_AC:
            os << std::right << std::setw(8) << code->A();
            os << "        "; // for align
            os << std::right << std::setw(8) << code->C();
            break;
        case Code::TYPE_ABC:
            os << std::right << std::setw(8) << code->A();
            os << std::right << std::setw(8) << code->B();
            os << std::right << std::setw(8) << code->C();
            break;
        case Code::TYPE_ABx:
            os << std::right << std::setw(8) << code->A();
            os << std::right << std::setw(16) << code->Bx();
            break;
        case Code::TYPE_ASBx:
            os << std::right << std::setw(8) << code->A();
            os << std::right << std::setw(16) << code->sBx();
            break;
    }
    os << "\x1b[34m    ; " << code->comment();
    os << "\x1b[39m";
}

auto Writer::writeConstant(std::ostream& os, const std::shared_ptr<Constant> constant) -> void
{
    os << "\x1b[39m";
    switch (constant->type()) {
        case Constant::NIL:
            os << "nil";
            break;
        case Constant::BOOLEAN:
            os << std::boolalpha << constant->get<bool>();
            break;
        case Constant::NUMBER:
            os << constant->get<lua_Number>();
            break;
        case Constant::INTEGER:
            os << constant->get<lua_Integer>();
            break;
        case Constant::STRING:
            os << '"' << constant->get<std::string>() << '"';
            break;
    }
}

auto Writer::writeUpValue(std::ostream& os, const std::shared_ptr<UpValue> upValue,
    const std::string& name) -> void
{
    os << "\x1b[39m";
    os << std::boolalpha << "inStack: " << upValue->inStack();
    os << ", index: " << upValue->index();
    os << "\x1b[34m    ; " << name;
    os << "\x1b[39m";
}

auto Writer::writeLocalVar(std::ostream& os, const DebugInfo::LocalVar& localVar) -> void
{
    os << "\x1b[39m";
    os << '"' << localVar.name << '"';
    os << " (" << localVar.startPC << " - " << localVar.endPC << ')';
    os << "\x1b[39m";
}

} // namespace lhades
