#include "IR/block.hpp"
#include "IR/instruction.hpp"
#include "IR/function.hpp"
#include "cast.hpp"

#include <algorithm>

namespace scbe::IR {

Block::~Block() = default;
Block::Block(Ref<Context> ctx, const std::string& name) : GlobalValue(ctx->makePointerType(ctx->getVoidType()), Linkage::Internal, ValueKind::Block, name), m_context(ctx) {}

bool nameBlacklist(Instruction::Opcode opcode) {
    return opcode == Instruction::Opcode::Store || opcode == Instruction::Opcode::Ret || opcode == Instruction::Opcode::Jump;
}

void Block::addInstruction(std::unique_ptr<Instruction> instruction) {
    if(instruction->getName().empty() && !nameBlacklist(instruction->getOpcode()))
        instruction->setName(std::to_string(m_parentFunction->m_valueNameCounter++));
    instruction->m_parentBlock = this;

    if(instruction->getOpcode() == Instruction::Opcode::Allocate)
        m_parentFunction->m_allocations.push_back(cast<AllocateInstruction>(instruction.get()));
    m_instructions.push_back(std::move(instruction));
}

void Block::addInstructionAtFront(std::unique_ptr<Instruction> instruction) {
    if(instruction->getName().empty() && !nameBlacklist(instruction->getOpcode()))
        instruction->setName(std::to_string(m_parentFunction->m_valueNameCounter++));
    instruction->m_parentBlock = this;
    if(instruction->getOpcode() == Instruction::Opcode::Allocate)
        m_parentFunction->m_allocations.push_back(cast<AllocateInstruction>(instruction.get()));
    m_instructions.insert(m_instructions.begin(), std::move(instruction));
}

void Block::addInstructionAfter(std::unique_ptr<Instruction> instruction, Instruction* after) {
    if(instruction->getName().empty() && !nameBlacklist(instruction->getOpcode()))
        instruction->setName(std::to_string(m_parentFunction->m_valueNameCounter++));
    instruction->m_parentBlock = this;

    auto it = getInstructionIdx(after);

    if(instruction->getOpcode() == Instruction::Opcode::Allocate)
        m_parentFunction->m_allocations.push_back(cast<AllocateInstruction>(instruction.get()));
    m_instructions.insert(it + 1, std::move(instruction));
}

void Block::addInstructionBefore(std::unique_ptr<Instruction> instruction, Instruction* before) {
    if(instruction->getName().empty() && !nameBlacklist(instruction->getOpcode()))
        instruction->setName(std::to_string(m_parentFunction->m_valueNameCounter++));
    instruction->m_parentBlock = this;

    auto it = getInstructionIdx(before);

    if(instruction->getOpcode() == Instruction::Opcode::Allocate)
        m_parentFunction->m_allocations.push_back(cast<AllocateInstruction>(instruction.get()));
    m_instructions.insert(it, std::move(instruction));
}

void Block::removeInstruction(Instruction* instruction) {
    auto idx = getInstructionIdx(instruction);
    if(idx == m_instructions.end()) return;

    if(auto jump = dyn_cast<IR::JumpInstruction>(instruction)) {
        Block* toBlock = cast<Block>(jump->getOperand(0));
        toBlock->removePredecessor(this);
        removeSuccessor(toBlock);

        if(jump->getNumOperands() > 1) {
            Block* elseBlock = cast<Block>(jump->getOperand(1));
            elseBlock->removePredecessor(this);
            removeSuccessor(elseBlock);
        } 

        m_parentFunction->m_dominatorTreeDirty = true;
    }
    else if(auto alloc = dyn_cast<IR::AllocateInstruction>(instruction)) {
        m_parentFunction->m_allocations.erase(std::find(m_parentFunction->m_allocations.begin(), m_parentFunction->m_allocations.end(), alloc));
    }
    
    for(auto& op : instruction->getOperands())
        op->removeFromUses(instruction);

    for(auto& instr : instruction->getUses())
        instr->removeOperand(instruction);
    
    m_instructions.erase(idx);
}

Block* Block::getImmediateDominator() {
    Block* idom = nullptr;
    for(auto d : m_dominators) {
        if(d == this) continue;
        bool isImmediate = true;
        for(auto other : m_dominators) {
            if(other == this || other == d) continue;
            if(std::find(other->m_dominators.begin(), other->m_dominators.end(), d) != other->m_dominators.end()) {
                isImmediate = false;
                break;
            }
        }
        if(isImmediate) {
            idom = d;
            break;
        }
    }
    return idom;
}


void Block::setPhiForValue(Value* value, PhiInstruction* phi) {
    m_phiForValues[value] = phi;
}

std::unique_ptr<Block> Block::split(Instruction* at) {
    if(m_instructions.size() < 2) return nullptr;
    std::unique_ptr<Block> block = std::unique_ptr<Block>(new Block(at->getParentBlock()->m_context));
    auto point = getInstructionIdx(at);
    // block->m_instructions.insert(block->m_instructions.begin(), point+1, m_instructions.end());
    block->m_instructions.insert(
        block->m_instructions.begin(),
        std::make_move_iterator(point + 1),
        std::make_move_iterator(m_instructions.end())
    );
    m_instructions.erase(point+1, m_instructions.end());
    for(auto& instruction : block->m_instructions) {
        instruction->m_parentBlock = block.get();
    }
    return std::move(block);
}

void Block::addSuccessor(Block* block) {
    if(!m_successors.contains(block)) {
        m_successors.insert({block, 1});
        return;
    }
    m_successors.at(block)++;
}

void Block::addPredecessor(Block* block) {
    if(!m_predecessors.contains(block)) {
        m_predecessors.insert({block, 1});
        return;
    }
    m_predecessors.at(block)++;
}

void Block::removeSuccessor(Block* block) {
    uint32_t& count = m_successors.at(block);
    count--;
    if(count == 0) m_successors.erase(block);
}

void Block::removePredecessor(Block* block) {
    uint32_t& count = m_predecessors.at(block);
    count--;
    if(count == 0) m_predecessors.erase(block);
}


}