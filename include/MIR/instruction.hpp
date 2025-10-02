#pragma once

#include "operand.hpp"
#include "type.hpp"
#include <cstdint>
#include <vector>

#define CALL_LOWER_OP INT_MAX
#define SWITCH_LOWER_OP CALL_LOWER_OP - 1
#define RETURN_LOWER_OP SWITCH_LOWER_OP - 1

namespace scbe::MIR {

class Instruction {
public:
    Instruction(uint32_t opcode) : m_opcode(opcode) {}
    template<typename... Args>
    Instruction(uint32_t opcode, Args&&... ops) : m_opcode(opcode) {
        (m_operands.push_back(std::forward<Args>(ops)), ...);
    }

    virtual ~Instruction() = default;

    uint32_t getOpcode() const { return m_opcode; }

    void addOperand(Operand* operand) { m_operands.push_back(operand); }
    std::vector<Operand*>& getOperands() { return m_operands; }
    MIR::Block* getParentBlock() const { return m_parentBlock; }

protected:
    uint32_t m_opcode;
    std::vector<Operand*> m_operands;
    MIR::Block* m_parentBlock = nullptr;

friend class MIR::Block;
};

class CallLowering : public Instruction {
public:
    CallLowering() : Instruction(CALL_LOWER_OP) {}

    void addType(Type* type) { m_types.push_back(type); }

    const std::vector<Type*>& getTypes() const { return m_types; }

private:
    std::vector<Type*> m_types;
};

class SwitchLowering : public Instruction {
public:
    SwitchLowering() : Instruction(SWITCH_LOWER_OP) {}

    MIR::Operand* getCondition() { return getOperands().at(0); }
    MIR::Block* getDefault() { return cast<MIR::Block>(getOperands().at(1)); }
    std::vector<std::pair<MIR::ImmediateInt*, MIR::Block*>> getCases() const;
};

class ReturnLowering : public Instruction {
public:
    ReturnLowering() : Instruction(RETURN_LOWER_OP) {}

    MIR::Operand* getValue() { return getOperands().at(0); }
};

class CallInstruction : public Instruction {
public:
    CallInstruction(uint32_t opcode) : Instruction(opcode) {}
    template<typename... Args>
    CallInstruction(uint32_t opcode, Args&&... ops) : Instruction(opcode, std::forward<Args>(ops)...) {}

    size_t getStartOffset() const { return m_startOffset; }
    size_t getEndOffset() const { return m_endOffset; }

    void setStartOffset(size_t offset) { m_startOffset = offset; }
    void setEndOffset(size_t offset) { m_endOffset = offset; }
    void addReturnRegister(uint32_t reg) { m_returnRegisters.push_back(reg); }

    const std::vector<uint32_t>& getReturnRegisters() const { return m_returnRegisters; }

private:
    size_t m_startOffset = 0;
    size_t m_endOffset = 0;

    std::vector<uint32_t> m_returnRegisters;
};
    
}