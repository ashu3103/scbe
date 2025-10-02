#pragma once

#include "pass.hpp"

namespace scbe::IR {

class ComputeDominators : public FunctionPass {
public:
    bool run(IR::Function* function) override;   
};

}