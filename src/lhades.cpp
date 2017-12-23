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

    if (LHADES_INT != load<lua_Integer>()) {
        throw std::string("endianness mismatch");
    }
    if (LHADES_NUM != load<lua_Number>()) {
        throw std::string("float format mismatch");
    }
}

auto LHades::checkLiteral(const std::string &str, const std::string &errMsg) -> void
{
    auto len = str.size();
    auto buf = new char[len + 1];
    m_ifs.read(buf, len);
    buf[len + 1] = '\0';

    if (std::strcmp(str.c_str(), buf) != 0) {
        throw errMsg;
    }
    delete[] buf;
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
    char buf = load<char>();
    if (buf != 0x00) {
        return std::to_string(buf);
    }
    return std::string("official");
}
