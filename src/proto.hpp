/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_PROTO_HPP
#define LHADES_PROTO_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace lhades {

class Code;
class Constant;
class UpValue;
class DebugInfo;

class Proto
{
public:
    static auto read(std::istream& is, const std::string& source = "")
        -> std::shared_ptr<Proto>;

public:
    Proto();
    Proto(const Proto& other);
    auto operator=(const Proto& other) -> const Proto&;
    ~Proto() = default;

    auto source() const -> std::string { return m_source; }
    auto lineDefined() const -> int { return m_lineDefined; }
    auto lastLineDefined() const -> int { return m_lastLineDefined; }
    auto numParams() const -> int { return m_numParams; }
    auto isVarArg() const -> int { return m_isVarArg; }
    auto maxStackSize() const -> int { return m_maxStackSize; }

    auto codes() const -> const std::vector<std::shared_ptr<Code>>& { return m_codes; }
    auto constants() const -> const std::vector<std::shared_ptr<Constant>>& { return m_constants; }
    auto upValues() const -> const std::vector<std::shared_ptr<UpValue>>& { return m_upValues; }
    auto protos() const -> const std::vector<std::shared_ptr<Proto>>& { return m_protos; }
    auto debugInfo() const -> const std::shared_ptr<DebugInfo> { return m_debugInfo; }

private:
    std::string m_source;
    int m_lineDefined;
    int m_lastLineDefined;
    int m_numParams;
    int m_isVarArg;
    int m_maxStackSize;
    std::vector<std::shared_ptr<Code>> m_codes;
    std::vector<std::shared_ptr<Constant>> m_constants;
    std::vector<std::shared_ptr<UpValue>> m_upValues;
    std::vector<std::shared_ptr<Proto>> m_protos;
    std::shared_ptr<DebugInfo> m_debugInfo;
};

} // namespace lhades

#endif // LHADES_PROTO_HPP
