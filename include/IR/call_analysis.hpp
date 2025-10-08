#pragma once

#include "pass.hpp"

namespace scbe::IR {

class Block;

class CallAnalysis : public FunctionPass {
public:
    bool run(Function* function) override;
};

}