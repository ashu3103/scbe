#include "IR/dce.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/instruction.hpp"

#include <deque>

namespace scbe::IR {

bool DeadCodeElimination::run(IR::Function* function) {
    std::deque<Instruction*> worklist;
    for (auto& bb : function->getBlocks()) {
        for (auto& inst : bb->getInstructions()) {
            if (isDead(inst.get()))
                worklist.push_back(inst.get());
        }
    }
    bool changed = !worklist.empty();

    while (!worklist.empty()) {
        Instruction* inst = worklist.back();
        worklist.pop_back();

        inst->getParentBlock()->removeInstruction(inst);

        for (Value* op : inst->getOperands()) {
            if (Instruction* opInst = dyn_cast<Instruction>(op)) {
                if (isDead(opInst))
                    worklist.push_back(opInst);
            }
        }
    }

    return changed;
}

bool DeadCodeElimination::isDead(IR::Instruction* instruction) {
    if(instruction->getOpcode() == IR::Instruction::Opcode::Store) {
        IR::Value* ptr = instruction->getOperands().at(0);
        if(auto ptrInst = dyn_cast<IR::Instruction>(ptr)) {
            if(ptrInst->getOpcode() != IR::Instruction::Opcode::Allocate)
                return false;
        }
        bool onlyStore = true;
        for(auto& use : ptr->getUses()) {
            if(use->getOpcode() != IR::Instruction::Opcode::Store)
                onlyStore = false;
        }
        return onlyStore;
    }
    
    return !instruction->hasSideEffect() && instruction->getUses().size() == 0;
}

}