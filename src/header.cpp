/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <istream>
#include <stdexcept>
#include <string>

#include "config.hpp"
#include "header.hpp"

namespace lhades {

auto Header::read(std::istream &is) -> void
{
    checkLiteral(is, "\x1bLua", "invalid signature");
    if (Reader::read<char>(is) != 0x53) { // check version
        throw std::runtime_error("only support lua 5.3");
    }
    Reader::read<char>(is); // skip format version
    checkLiteral(is, "\x19\x93\r\n\x1a\n", "conversion error");

    checkSize<int>(is);
    checkSize<std::size_t>(is);
    checkSize<Instruction>(is);
    checkSize<lua_Integer>(is);
    checkSize<lua_Number>(is);

    if (Reader::read<lua_Integer>(is) != 0x5678) {
        throw std::runtime_error("endianness mismatch");
    }
    if (Reader::read<lua_Number>(is) != (lua_Number)370.5) {
        throw std::runtime_error("float format mismatch");
    }
}

auto Header::checkLiteral(std::istream& is, const std::string &str, const std::string &errMsg) -> void
{
    auto len = str.size();
    std::string buf(len, '\0');
    is.read(&buf[0], len);

    if (str != buf) {
        throw std::runtime_error(errMsg);
    }
}


} // namespace lhades
