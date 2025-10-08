#include "IR/instruction.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/dominator_tree.hpp"

namespace scbe::IR {

DominatorTree::DominatorTree(Function* function) {
    for(const auto& block : function->getBlocks()) {
        if(!block->getImmediateDominator()) continue;
        m_dominatorTree[block->getImmediateDominator()].push_back(block.get());
    }
}

bool DominatorTree::dominates(Block* dominator, Block* dominated) const {
    if(!hasChildren(dominator)) return false;
    auto& children = m_dominatorTree.at(dominator);
    return std::find(children.begin(), children.end(), dominated) != children.end();
}

}