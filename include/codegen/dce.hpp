#pragma once

#include "pass.hpp"

namespace scbe::Codegen {

class DeadCodeElimination : public FunctionPass {
public:
    bool run(IR::Function* function) override;

private:
    bool isDead(IR::Instruction* instruction);
};

}