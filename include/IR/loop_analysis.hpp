#pragma once

#include "pass.hpp"

namespace scbe::IR {

class Block;
class LoopInfo;

class LoopAnalysis : public FunctionPass {
public:
    bool run(IR::Function* function) override;
    void propagateDepth(LoopInfo* loop, uint32_t depth);
};

}