/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_CONSTANT_HPP
#define LHADES_CONSTANT_HPP

#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <lua.hpp>

namespace lhades {

class Constant
{
public:
    enum Type {
        BOOLEAN = 0x00,
        NUMBER  = 0x01,
        INTEGER = 0x02,
        STRING  = 0x03,
    };

    static auto read(std::istream &is) -> std::shared_ptr<Constant>;

    Constant();
    Constant(const Constant& other);
    auto operator=(const Constant& other) -> const Constant&;
    ~Constant() = default;

public:
    template <class T>
    auto get() const
        -> typename std::enable_if<std::is_same<T, bool>::value, T>::type {
        if (m_type != Type::BOOLEAN) throw std::runtime_error("type error");
        return m_bValue;
    }

    template <class T>
    auto get() const
        -> typename std::enable_if<std::is_same<T, lua_Number>::value, T>::type {
        if (m_type != Type::NUMBER) throw std::runtime_error("type error");
        return m_nValue;
    }

    template <class T>
    auto get() const
        -> typename std::enable_if<std::is_same<T, lua_Integer>::value, T>::type {
        if (m_type != Type::INTEGER) throw std::runtime_error("type error");
        return m_iValue;
    }

    template <class T>
    auto get() const
        -> typename std::enable_if<std::is_same<T, std::string>::value, T>::type {
        if (m_type != Type::STRING) throw std::runtime_error("type error");
        return m_sValue;
    }

    auto type() const -> Type { return m_type; }

private:
    Type m_type;
    bool m_bValue;
    lua_Number m_nValue;
    lua_Integer m_iValue;
    std::string m_sValue;
};

} // namespace lhades

#endif // LHADES_CONSTANT_HPP
