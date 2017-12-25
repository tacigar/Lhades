/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <istream>
#include <memory>

#include "reader.hpp"
#include "upvalue.hpp"

namespace lhades {

auto UpValue::read(std::istream &is) -> std::shared_ptr<UpValue>
{
    auto upValue = std::make_shared<UpValue>();
    upValue->m_inStack = static_cast<bool>(Reader::read<char>(is));
    upValue->m_index = static_cast<int>(Reader::read<char>(is));
    return upValue;
}

UpValue::UpValue()
{
}

UpValue::UpValue(const UpValue& other)
    : m_inStack(other.m_inStack)
    , m_index(other.m_index)
{
}

auto UpValue::operator=(const UpValue& other) -> const UpValue&
{
    m_inStack = other.m_inStack;
    m_index = other.m_index;
    return *this;
}

} // namespace lhades
