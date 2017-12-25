/*
** lhades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_WRITER_HPP
#define LHADES_WRITER_HPP

#include <memory>
#include <ostream>

#include "debug_info.hpp"

namespace lhades {

class Code;
class Constant;
class Proto;
class UpValue;

class Writer
{
public:
    static auto write(std::ostream& os, const std::shared_ptr<Proto> proto) -> void;

private:
    static auto writeSpace(int leve, std::ostream& os) -> void;
    static auto writeProto(int level, std::ostream& os, const std::shared_ptr<Proto> proto) -> void;
    static auto writeCode(std::ostream& os, const std::shared_ptr<Code> code) -> void;
    static auto writeConstant(std::ostream& os, const std::shared_ptr<Constant> constant) -> void;
    static auto writeUpValue(std::ostream& os, const std::shared_ptr<UpValue> upValue,
        const std::string& name) -> void;
    static auto writeLocalVar(std::ostream& os, const DebugInfo::LocalVar& localVar) -> void;
};

} // namespace lhades

#endif // LHADES_WRITER_HPP
