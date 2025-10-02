#pragma once

#include "pass.hpp"

namespace scbe {
class Context;
}

namespace scbe::Target::x64 {

class x64Legalizer : public InstructionPass {
public:
    x64Legalizer(Ref<Context> context) : m_context(context) {}

    virtual bool run(IR::Instruction* instruction) override;

private:
    Ref<Context> m_context = nullptr;;
};

}