/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <iostream>

#include "code.hpp"
#include "constant.hpp"
#include "debug_info.hpp"
#include "proto.hpp"
#include "upvalue.hpp"
#include "reader.hpp"

namespace lhades {

Proto::Proto()
{
}

Proto::Proto(const Proto& other)
    : m_lineDefined(other.m_lineDefined)
    , m_lastLineDefined(other.m_lastLineDefined)
    , m_numParams(other.m_numParams)
    , m_isVarArg(other.m_isVarArg)
    , m_maxStackSize(other.m_maxStackSize)
    , m_codes(other.m_codes)
    , m_constants(other.m_constants)
    , m_upValues(other.m_upValues)
    , m_protos(other.m_protos)
{
}

auto Proto::operator=(const Proto& other) -> const Proto&
{
    m_lineDefined = other.m_lineDefined;
    m_lastLineDefined = other.m_lastLineDefined;
    m_numParams = other.m_numParams;
    m_isVarArg = other.m_isVarArg;
    m_maxStackSize = other.m_maxStackSize;
    m_codes = other.m_codes;
    m_constants = other.m_constants;
    m_upValues = other.m_upValues;
    m_protos = other.m_protos;

    return *this;
}

auto Proto::read(std::istream &is, const std::string &source)
    -> std::shared_ptr<Proto>
{
    auto proto = std::make_shared<Proto>();

    auto src = Reader::readString(is);
    if (src == "") {
        src = source;
    }
    proto->m_source = src;
    proto->m_lineDefined = Reader::read<int>(is);
    proto->m_lastLineDefined = Reader::read<int>(is);
    proto->m_numParams = static_cast<int>(Reader::read<char>(is));
    proto->m_isVarArg = static_cast<int>(Reader::read<char>(is));
    proto->m_maxStackSize = static_cast<int>(Reader::read<char>(is));

    auto codeSize = Reader::read<int>(is);
    for (int i = 0; i < codeSize; ++i) {
        auto code = Code::read(is);
        proto->m_codes.push_back(code);
    }

    auto constSize = Reader::read<int>(is);
    for (int i = 0; i < constSize; ++i) {
        auto constant = Constant::read(is);
        proto->m_constants.push_back(constant);
    }

    auto upValueSize = Reader::read<int>(is);
    for (int i = 0; i < upValueSize; ++i) {
        auto upValue = UpValue::read(is);
        proto->m_upValues.push_back(upValue);
    }

    auto protoSize = Reader::read<int>(is);
    for (int i = 0; i < protoSize; ++i) {
        auto p = Proto::read(is, proto->m_source);
        proto->m_protos.push_back(p);
    }

    proto->m_debugInfo = DebugInfo::read(is);

    return proto;
}

} // namespace lhades
