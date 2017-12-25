/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <istream>
#include <memory>
#include <lua.h>

#include "config.hpp"
#include "constant.hpp"
#include "reader.hpp"

namespace lhades {

auto Constant::read(std::istream& is) -> std::shared_ptr<Constant>
{
    auto constant = std::make_shared<Constant>();

    auto type = static_cast<int>(Reader::read<char>(is));
    switch (type) {
        case LUA_TBOOLEAN: {
            auto b = static_cast<bool>(Reader::read<char>(is));
            constant->m_type = Type::BOOLEAN;
            constant->m_bValue = b;
            break;
        }
        case LUA_TNUMFLT: {
            auto n = Reader::read<lua_Number>(is);
            constant->m_type = Type::NUMBER;
            constant->m_nValue = n;
            break;
        }
        case LUA_TNUMINT: {
            auto i = Reader::read<lua_Integer>(is);
            constant->m_type = Type::INTEGER;
            constant->m_iValue = i;
            break;
        }
        case LUA_TSHRSTR: case LUA_TLNGSTR: {
            auto s = Reader::readString(is);
            constant->m_type = Type::STRING;
            constant->m_sValue = s;
            break;
        }
        default:
            throw std::runtime_error("no such type value");
            break;
    }
    return constant;
}

Constant::Constant()
{
}

Constant::Constant(const Constant& other)
    : m_type(other.m_type)
    , m_bValue(other.m_bValue)
    , m_nValue(other.m_nValue)
    , m_iValue(other.m_iValue)
    , m_sValue(other.m_sValue)
{
}

auto Constant::operator=(const Constant& other) -> const Constant&
{
    m_type = other.m_type;
    m_bValue = other.m_bValue;
    m_nValue = other.m_nValue;
    m_iValue = other.m_iValue;
    m_sValue = other.m_sValue;

    return *this;
}

} // namespace lhades
