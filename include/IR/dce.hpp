#pragma once

#include "pass.hpp"

namespace scbe::IR {

class DeadCodeElimination : public FunctionPass {
public:
    bool run(IR::Function* function) override;

private:
    bool isDead(IR::Instruction* instruction);
};

}