/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <cstdlib>
#include <istream>
#include <stdexcept>

#include "reader.hpp"

namespace lhades {

class Header
{
public:
    static auto read(std::istream &is) -> void;

private:
    template <class T>
    static auto checkSize(std::istream& is) -> void {
        if (sizeof(T) != static_cast<std::size_t>(Reader::read<char>(is))) {
            throw std::runtime_error("invalid size");
        }
    }

    static auto checkLiteral(std::istream& is, const std::string &str,
        const std::string &errMsg) -> void;
};

} // namespace lhades
