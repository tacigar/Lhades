/*
** LHades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_LHADES_HPP
#define LHADES_LHADES_HPP

#include <fstream>
#include <sstream>
#include <string>

class LHades
{
public:
    LHades(const char *filename);
    auto disassemble() -> std::string;

private:
    auto header() -> void;
    auto checkLiteral(const std::string &str, const std::string &errMsg) -> void;
    auto luaVersion() -> std::string;
    auto formatVersion() -> std::string;

    template <class T>
    auto load() -> T
    {
        T buf;
        m_ifs.read((char *)&buf, sizeof(T));
        return buf;
    }

    template <class T>
    auto checkSize() -> void
    {
        if (sizeof(T) != static_cast<int>(load<char>())) {
            throw std::string("invalid size");
        }
    }

private:
    std::string m_filename;
    std::ifstream m_ifs;
    std::stringstream m_ss;
};

#endif // LHADES_LHADES_HPP
