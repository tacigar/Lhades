/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <istream>
#include <memory>
#include <string>
#include <vector>

#include "debug_info.hpp"
#include "reader.hpp"

namespace lhades {

auto DebugInfo::read(std::istream &is) -> std::shared_ptr<DebugInfo>
{
    auto debugInfo = std::make_shared<DebugInfo>();

    auto sizeLineInfo = Reader::read<int>(is);
    for (int i = 0; i < sizeLineInfo; ++i) {
        auto n = Reader::read<int>(is);
        debugInfo->m_lineInfo.push_back(n);
    }

    auto sizeLocalVars = Reader::read<int>(is);
    for (int i = 0; i < sizeLocalVars; ++i) {
        auto varName = Reader::readString(is);
        auto startPC = Reader::read<int>(is);
        auto endPC = Reader::read<int>(is);

        LocalVar localVar = {varName, startPC, endPC};
        debugInfo->m_localVars.push_back(localVar);
    }

    auto sizeUpValues = Reader::read<int>(is);
    for (int i = 0; i < sizeUpValues; ++i) {
        auto upValue = Reader::readString(is);
        debugInfo->m_upValues.push_back(upValue);
    }
    return debugInfo;
}

DebugInfo::DebugInfo()
{
}

DebugInfo::DebugInfo(const DebugInfo& other)
    : m_lineInfo(other.m_lineInfo)
    , m_localVars(other.m_localVars)
    , m_upValues(other.m_upValues)
{
}

auto DebugInfo::operator=(const DebugInfo& other) -> const DebugInfo&
{
    m_lineInfo = other.m_lineInfo;
    m_localVars = other.m_localVars;
    m_upValues = other.m_upValues;
    return *this;
}

} // namespace lhades
