#pragma once

namespace scbe::IR {

class Function;
class Block;

class DominatorTree {
public:
    DominatorTree(Function* function);

    const std::vector<Block*>& getChildren(Block* block) const { return m_dominatorTree.at(block); }
    bool hasChildren(Block* block) const { return m_dominatorTree.contains(block); }
private:
    std::unordered_map<Block*, std::vector<Block*>> m_dominatorTree;
};

}