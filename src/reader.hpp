/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_READER_HPP
#define LHADES_READER_HPP

#include <istream>
#include <string>

namespace lhades {

class Reader
{
public:
    template <class T>
    static auto read(std::istream& is) -> T
    {
        T res;
        is.read((char *)&res, sizeof(T));
        return res;
    }

    static auto readString(std::istream& is) -> std::string;
};

} // namespace lhades

#endif // LHADES_READER_HPP
