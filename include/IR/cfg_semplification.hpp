#pragma once

#include "pass.hpp"

namespace scbe::IR {

class CFGSemplification : public FunctionPass {
public:
    bool run(IR::Function* function) override;

    bool mergeBlocks(IR::Function* function);
    bool removeNoPredecessors(IR::Function* function);
    bool replaceEmpty(IR::Function* function);
};

}