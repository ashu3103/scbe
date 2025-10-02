#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/instruction.hpp"
#include "IR/dominator_tree.hpp"
#include "codegen/compute_dominators.hpp"
#include "unit.hpp"

#include <algorithm>
#include <cassert>

namespace scbe::IR {

Function::Function(const std::string& name, FunctionType* type, Linkage linkage): GlobalValue(type, linkage, ValueKind::Function, name) {
    for(size_t i = 0; i < type->getArguments().size(); i++) {
        auto arg = type->getArguments()[i];
        auto ptr = std::make_unique<FunctionArgument>(std::to_string(m_valueNameCounter++), arg, i);
        m_args.push_back(std::move(ptr));
    }
}

Function::~Function() = default;

Block* Function::insertBlock(const std::string name) {
    auto block = std::unique_ptr<Block>(new Block(m_unit->getContext(), name));
    Block* ret = block.get();
    insertBlock(std::move(block));
    return ret;
}

Block* Function::insertBlockAfter(Block* after, const std::string name) {
    auto block = std::unique_ptr<Block>(new Block(m_unit->getContext(), name));
    Block* ret = block.get();
    insertBlockAfter(after, std::move(block));
    return ret;
}

Block* Function::insertBlockBefore(Block* before, const std::string name) {
    auto block = std::unique_ptr<Block>(new Block(m_unit->getContext(), name));
    Block* ret = block.get();
    insertBlockBefore(before, std::move(block));
    return ret;
}

void Function::insertBlock(std::unique_ptr<Block> block) {
    if(block->getName().empty())
        block->setName(".");

    if(!m_unit->m_blockNameStack.contains(block->getName()))
        m_unit->m_blockNameStack[block->getName()] = 0;
    block->setName(block->getName() + std::to_string(m_unit->m_blockNameStack[block->getName()]++));

    block->m_parentFunction = this;
    m_blocks.push_back(std::move(block));
}

void Function::insertBlockAfter(Block* after, std::unique_ptr<Block> block) {
    if(block->getName().empty())
        block->setName(".");

    if(!m_unit->m_blockNameStack.contains(block->getName()))
        m_unit->m_blockNameStack[block->getName()] = 0;
    block->setName(block->getName() + std::to_string(m_unit->m_blockNameStack[block->getName()]++));

    block->m_parentFunction = this;

    auto it = getBlockIdx(after);
    m_blocks.insert(it + 1, std::move(block));
}

void Function::insertBlockBefore(Block* before, std::unique_ptr<Block> block) {
    if(block->getName().empty())
        block->setName(".");

    if(!m_unit->m_blockNameStack.contains(block->getName()))
        m_unit->m_blockNameStack[block->getName()] = 0;
    block->setName(block->getName() + std::to_string(m_unit->m_blockNameStack[block->getName()]++));

    block->m_parentFunction = this;

    auto it = getBlockIdx(before);
    m_blocks.insert(it, std::move(block));
}

Type* Function::getType() const {
    return m_unit->getContext()->makePointerType(m_type);
}

Block* Function::getEntryBlock() const {
    assert(!m_blocks.empty() && "Function has no blocks!");
    return m_blocks.front().get();
}

void Function::replace(Value* replace, Value* with) {
    for(auto& block : m_blocks) {
        for(auto& instr : block->getInstructions()) {
            for(auto& op : instr->m_operands) {
                if(op != replace) continue;
                replace->m_uses.erase(std::remove(replace->m_uses.begin(), replace->m_uses.end(), instr.get()), replace->m_uses.end());
                op = with;
                with->addUse(instr.get());
            }
        }
    }
}

void Function::removeInstruction(Instruction* instruction) {
    for(auto& block : m_blocks) {
        block->removeInstruction(instruction);
    }
}

void Function::computeDominatorTree() {
    Codegen::ComputeDominators().run(this);
    m_dominatorTree = std::make_unique<DominatorTree>(this);
}


void Function::removeBlock(Block* block) {
    for(auto& predecessor : block->m_predecessors)
        predecessor.first->m_successors.erase(block); // remove all instances of block

    for(auto& successor : block->m_successors)
        successor.first->m_predecessors.erase(block);
    
    m_blocks.erase(std::remove_if(m_blocks.begin(), m_blocks.end(), [&](auto& blockPtr) { return blockPtr.get() == block; }), m_blocks.end());
}

}