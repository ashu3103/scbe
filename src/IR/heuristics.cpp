#include "IR/heuristics.hpp"

namespace scbe::IR {

LoopInfo* Heuristics::getInnermostLoop(Block* forBlock) {
    std::sort(m_loops.begin(), m_loops.end(), [](auto& a, auto& b) { return a->getDepth() > b->getDepth(); });
    for(auto& loop : m_loops) {
        if(loop->contains(forBlock)) return loop.get();
    }
    return nullptr;
}

LoopInfo* Heuristics::getOutermostLoop(Block* forBlock) {
    std::sort(m_loops.begin(), m_loops.end(), [](auto& a, auto& b) { return a->getDepth() < b->getDepth(); });
    for(auto& loop : m_loops) {
        if(loop->contains(forBlock)) return loop.get();
    }
    return nullptr;
}


}