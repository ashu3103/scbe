#pragma once

#include <ostream>

namespace scbe::Target {
class InstructionInfo;
class RegisterInfo;
}

namespace scbe::MIR {
class Function;
class Block;
class Instruction;
class Operand;
}

namespace scbe {
class Unit;
}

namespace scbe::MIR {

class Printer {
public:
    Printer(std::ostream& output, Target::InstructionInfo* instructionInfo, Target::RegisterInfo* registerInfo) : m_output(output), m_instructionInfo(instructionInfo), m_registerInfo(registerInfo) {}

    virtual void print(Unit& unit) = 0;
    virtual void print(MIR::Function* function) = 0;
    virtual void print(MIR::Block* block) = 0;
    virtual void print(MIR::Instruction* instruction) = 0;
    virtual void print(MIR::Operand* operand) = 0;

protected:
    std::ostream& m_output;
    Target::InstructionInfo* m_instructionInfo;
    Target::RegisterInfo* m_registerInfo;

    Unit* m_unit = nullptr;
    MIR::Function* m_current = nullptr;
};

class HumanPrinter : public Printer {
public:
    HumanPrinter(std::ostream& output, Target::InstructionInfo* instructionInfo, Target::RegisterInfo* registerInfo) : Printer(output, instructionInfo, registerInfo) {}
    void print(Unit& unit) override;
    void print(MIR::Function* function) override;
    void print(MIR::Block* block) override;
    void print(MIR::Instruction* instruction) override;
    void print(MIR::Operand* operand) override;
};

}