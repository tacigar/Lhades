/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <cstdlib>
#include <istream>
#include <string>

#include "reader.hpp"

namespace lhades {

auto Reader::readString(std::istream& is) -> std::string
{
    std::size_t size = read<char>(is);
    if (size == 0xFF) {
        size = read<std::size_t>(is);
    }

    if (size == 0) {
        return std::string();
    } else {
        std::string res(size - 1, '\0');
        is.read(&res[0], size - 1);
        return res;
    }
}

} // namespace lhades
