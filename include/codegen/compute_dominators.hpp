#pragma once

#include "pass.hpp"

namespace scbe::Codegen {

class ComputeDominators : public FunctionPass {
public:
    bool run(IR::Function* function) override;   
};

}