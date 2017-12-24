/*
** LHades
** Copyright (C) 2017 tacigar
*/

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <lua.hpp>
#include <luaconf.h>

#include "config.hpp"
#include "lhades.hpp"
#include "opcode.hpp"

LHades::LHades(const char *filename)
    : m_filename(filename)
    , m_ifs()
    , m_ss()
{
}

auto LHades::disassemble() -> std::string
{
    m_ifs.open(m_filename, std::ios::in | std::ios::binary);
    if (m_ifs.fail()) {
        std::cerr << "cannot open the file : " << m_filename << std::endl;
        std::exit(1);
    }

    try {
        header();
        load<char>();
        function();

    } catch(std::string &errMsg) {
        m_ifs.close();
        std::cerr << errMsg << std::endl;
        exit(1);
    }

    m_ifs.close();
    return m_ss.str();
}

auto LHades::header() -> void
{
    checkLiteral(LHADES_SIGNATURE, "invalid signature");
    m_ss << "\nLuaVersion: " << luaVersion()
         << "\nFormatVersion: " << formatVersion();
    checkLiteral(LHADES_DATA, "conversion error");

    checkSize<int>();
    checkSize<std::size_t>();
    checkSize<Instruction>();
    checkSize<lua_Integer>();
    checkSize<lua_Number>();

    {
        lua_Integer buf;
        m_ifs.read((char *)&buf, sizeof(lua_Integer));
        if (LHADES_INT != buf) {
            throw std::string("endianness mismatch");
        }
    }

    {
        lua_Number buf;
        m_ifs.read((char *)&buf, sizeof(lua_Number));
        if (LHADES_NUM != buf) {
            throw std::string("float format mismatch");
        }
    }
}

auto LHades::checkLiteral(const std::string &str, const std::string &errMsg) -> void
{
    auto len = str.size();
    std::string buf(len, '\0');
    m_ifs.read(&buf[0], len);

    if (str != buf) {
        throw errMsg;
    }
}

auto LHades::luaVersion() -> std::string
{
    if (load<char>() != 0x53) {
        throw std::string("only support lua 5.3");
    }
    return std::string("Lua 5.3");
}

auto LHades::formatVersion() -> std::string
{
    auto buf = load<char>();
    if (buf != 0x00) {
        return std::to_string(buf);
    }
    return std::string("official");
}

auto LHades::function() -> void
{
    auto source = loadString();

    auto lineDefined = load<int>();
    auto lastLineDefined = load<int>();
    auto numParams = static_cast<int>(load<char>());
    auto isVararg = static_cast<bool>(load<char>());
    auto maxStackSize = static_cast<int>(load<char>());

    m_ss << "\nfunction name \"" << source << "\", "
         << "[" << lineDefined << " - " << lastLineDefined << "]"
         << numParams << "params, "
         << maxStackSize << "stacks";

    loadCode();
    loadConstants();
    loadUpValues();
    loadProtos();
}

auto LHades::loadString() -> std::string
{
    std::size_t size = load<char>();
    if (size == 0xFF) {
        size = load<std::size_t>();
    }

    if (size == 0) {
        return std::string();
    } else if (size == 1) {
        load<char>();
        return std::string();
    } else {
        std::string res(size, '\0');
        m_ifs.read(&res[0], size - 1);
        return res;
    }
}

auto LHades::loadCode() -> void
{
    auto codeSize = load<int>();
    m_ss << ", " << codeSize << "codes";

    for (int i = 0; i < codeSize; ++i) {
        auto code = OpCode::genCode(load<Instruction>());
        m_ss << "\n  [" << i + 1 << "]  " << code;
    }
}

auto LHades::loadConstants() -> void
{
    m_ss << "\n.const";
    auto constantsSize = load<int>();

    for (int i = 0; i < constantsSize; ++i) {
        auto type = static_cast<int>(load<char>());

        switch (type) {
            case LUA_TBOOLEAN: {
                auto b = load<char>();
            }
            case LUA_TNUMFLT: {
                std::cout << "KOKpppO" << std::endl;
            }
            case LUA_TNUMINT: {
                auto n = load<lua_Integer>();
                m_ss << "\n  [" << i << "] " << n;
                break;
            }
            case LUA_TSHRSTR:
            case LUA_TLNGSTR: {
                auto s = loadString();
                m_ss << "\n  [" << i << "] " << s;
                break;
            }
            default:
                break;
        }
    }
}

auto LHades::loadUpValues() -> void
{
    m_ss << "\n.upvalues";
    auto upValuesSize = load<int>();

    for (int i = 0; i < upValuesSize; ++i) {
        auto s = loadString();
        m_ss << "\n  [" << i << "] " << s;
    }
}

auto LHades::loadProtos() -> void
{
    m_ss << "\n.protos";
    auto protosSize = static_cast<int>(load<int>());
    std::cout << protosSize << std::endl;

    for (int i = 0; i < protosSize; ++i) {
        m_ss << "\n  [" << i << "] ";
        function();
    }
}
