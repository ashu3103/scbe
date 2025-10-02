#include "IR/instruction.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include <span>

namespace scbe::IR {

JumpInstruction::JumpInstruction(Block* first, Block* second, Value* value) : Instruction(Opcode::Jump, nullptr, "") { addOperand(first); addOperand(second); addOperand(value); }
JumpInstruction::JumpInstruction(Block* first) : Instruction(Opcode::Jump, nullptr, "") { addOperand(first); }

CallInstruction::CallInstruction(Type* type, Value* callee, std::string name) : Instruction(Opcode::Call, type, name) { addOperand(callee); }
CallInstruction::CallInstruction(Type* type, Value* callee, const std::vector<Value*>& args, std::string name) : Instruction(Opcode::Call, type, name) {
    addOperand(callee);
    for(auto arg : args) addOperand(arg);
}

bool Instruction::hasSideEffect() const {
    return m_opcode == Opcode::Ret ||
    m_opcode == Opcode::Switch ||
    m_opcode == Opcode::Store ||
    m_opcode == Opcode::Jump ||
    m_opcode == Opcode::Call;
}

SwitchInstruction::SwitchInstruction(Value* value, Block* defaultCase, std::vector<std::pair<ConstantInt*, Block*>> cases) : Instruction(Instruction::Opcode::Switch, nullptr, "") {
    addOperand(value);
    addOperand(defaultCase);
    for(auto casePair : cases) {
        addOperand(casePair.first);
        addOperand(casePair.second);
    }
}

Block* SwitchInstruction::getDefaultCase() const {
    return cast<Block>(getOperand(1));
}

std::vector<std::pair<ConstantInt*, Block*>> SwitchInstruction::getCases() const {
    std::vector<std::pair<ConstantInt*, Block*>> cases;
    for(int i = 2; i < m_operands.size(); i += 2) {
        cases.emplace_back(cast<ConstantInt>(m_operands[i]), cast<Block>(m_operands[i + 1]));
    }
    return cases;
}

ConstantInt* ExtractValueInstruction::getIndex() const { return cast<ConstantInt>(getOperand(1)); }

}