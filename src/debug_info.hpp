/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_DEBUG_INFO_HPP
#define LHADES_DEBUG_INFO_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace lhades {

class DebugInfo
{
public:
    struct LocalVar
    {
        std::string name;
        int startPC;
        int endPC;
    };

    static auto read(std::istream &is) -> std::shared_ptr<DebugInfo>;

    DebugInfo();
    DebugInfo(const DebugInfo& other);
    auto operator=(const DebugInfo& other) -> const DebugInfo&;
    ~DebugInfo() = default;

    auto lineInfo() const -> const std::vector<int>& { return m_lineInfo; }
    auto localVars() const -> const std::vector<LocalVar>& { return m_localVars; }
    auto upValues() const -> const std::vector<std::string>& { return m_upValues; }

private:
    std::vector<int> m_lineInfo;
    std::vector<LocalVar> m_localVars;
    std::vector<std::string> m_upValues;
};

} // namespace lhades

#endif // LHADES_DEBUG_INFO_HPP
