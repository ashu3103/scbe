#pragma once

#include "pass.hpp"
namespace scbe::IR {

class FunctionInlining : public FunctionPass {
public:
    void init(Unit& unit) override { m_totalInstructionsAdded = 0; }

    bool run(Function* function) override;

private:
    size_t m_totalInstructionsAdded = 0;
};

}