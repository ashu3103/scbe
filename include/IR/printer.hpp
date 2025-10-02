#pragma once

#include "IR/instruction.hpp"

#include "type.hpp"

#include <ostream>

namespace scbe {
    class Unit;
}

namespace scbe::IR {

class Block;

class Printer {
public:
    virtual void print(std::ostream& os, Unit& unit) = 0;
    virtual void print(std::ostream& os, const Function* function) = 0;
    virtual void print(std::ostream& os, const Block* block) = 0;
    virtual void print(std::ostream& os, const Instruction* instruction) = 0;
    virtual void print(std::ostream& os, Instruction::Opcode opcode) = 0;
    virtual void print(std::ostream& os, const Value* value) = 0;
    virtual void print(std::ostream& os, const Type* type) = 0;

    void printIndentation(std::ostream& os);

protected:
    uint32_t m_indent = 0;
};

class HumanPrinter : public Printer {
public:
    void print(std::ostream& os, Unit& unit) override;
    void print(std::ostream& os, const Function* function) override;
    void print(std::ostream& os, const Block* block) override;
    void print(std::ostream& os, const Instruction* instruction) override;
    void print(std::ostream& os, Instruction::Opcode opcode) override;
    void print(std::ostream& os, const Value* value) override;
    void print(std::ostream& os, const Type* type) override;

private:
    USet<const StructType*> m_deferPrintStruct;
};

}