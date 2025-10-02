#pragma once

#include "pass.hpp"

namespace scbe::IR {
class DominatorTree;
class Block;
class AllocateInstruction;
}

namespace scbe::Codegen {

class Mem2Reg : public FunctionPass {
public:
    bool run(IR::Function* function) override;

    void rename(IR::DominatorTree* tree, IR::Block* current, UMap<IR::Value*, std::vector<IR::Value*>>& stack, const std::vector<IR::AllocateInstruction*>& promoted);
    bool isAllocaPromotable(IR::AllocateInstruction* instruction);
    bool wasAllocaUsed(const std::vector<IR::AllocateInstruction*>& promoted, IR::Instruction* instruction);

    USet<IR::Block*> allocaDefiningBlocks(IR::AllocateInstruction* instruction);
    USet<IR::Block*> allocaUsingBlocks(IR::AllocateInstruction* instruction);
};

}