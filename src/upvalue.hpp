/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_UPVALUE_HPP
#define LHADES_UPVALUE_HPP

#include <istream>
#include <memory>

namespace lhades {

class UpValue
{
public:
    static auto read(std::istream &is) -> std::shared_ptr<UpValue>;

    UpValue();
    UpValue(const UpValue& other);
    auto operator=(const UpValue& other) -> const UpValue&;
    ~UpValue() = default;

    auto inStack() const -> bool { return m_inStack; }
    auto index() const -> int { return m_index; }

private:
    bool m_inStack;
    int m_index;
};

} // namespace lhades

#endif // LHADES_UPVALUE_HPP
