#include "IR/compute_dominators.hpp"
#include "IR/dominator_tree.hpp"
#include "IR/instruction.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"

namespace scbe::IR {

bool ComputeDominators::run(IR::Function* function) {
    //dominators
    auto& blocks = function->getBlocks();
    for(auto& block : blocks) {
        block->m_dominators.clear();
        block->m_dominated.clear();
        block->m_dominanceFrontiers.clear();
        // block->m_dominators.insert(block->m_dominators.begin(), blocks.begin(), blocks.end());
        for(auto& other : blocks) {
            block->m_dominators.push_back(other.get());
        }
    }
    blocks.at(0)->m_dominators.clear(); blocks.at(0)->m_dominators.push_back(blocks.at(0).get());
    bool changed = false;
    do {
        changed = false;
        for(size_t i = 1; i < blocks.size(); i++) {
            auto& b = blocks.at(i);
            UMap<IR::Block*, size_t> appearence;
            std::vector<IR::Block*> ordered;
            std::vector<IR::Block*> newDominators;

            for(auto& predecessor : b->m_predecessors) {
                for(auto d : predecessor.first->m_dominators) {
                    if(!appearence.contains(d)) {
                        appearence.insert({d, 0});
                        ordered.push_back(d);
                    }
                    appearence[d]++;
                }
            }

            for(auto ord : ordered) {
                if(appearence.at(ord) != b->m_predecessors.size())
                    continue;
                newDominators.push_back(ord);
            }
            newDominators.push_back(b.get());

            if(newDominators != b->m_dominators) {
                b->m_dominators = std::move(newDominators);
                changed = true;
            }
        }
    }
    while(changed);

    for(auto& block : blocks) {
        for(auto dominator : block->m_dominators) {
            dominator->m_dominated.push_back(block.get());
        }
    }

    // dominance frontiers
    for(auto& block : blocks) {
        if(block->m_predecessors.size() < 2)
            continue;
        for(auto predecessor : block->m_predecessors) {
            IR::Block* runner = predecessor.first;
            IR::Block* idom = block->getImmediateDominator();
            while(runner && runner != idom) {
                runner->m_dominanceFrontiers.insert(block.get());
                runner = runner->getImmediateDominator();
            }
        }
    }

    return true;
}

}