#pragma once

#include "value.hpp"
#include "type.hpp"

#include <cassert>

namespace scbe::IR {

class Block;
class Function;

class Instruction : public Value {
public:
    enum class Opcode {
        Ret,
        Allocate,
        Load,
        Store,
        Add,
        Sub,
        ICmpEq,
        ICmpNe,
        ICmpGt,
        ICmpGe,
        ICmpLt,
        ICmpLe,
        UCmpGt,
        UCmpGe,
        UCmpLt,
        UCmpLe,
        FCmpEq,
        FCmpNe,
        FCmpGt,
        FCmpGe,
        FCmpLt,
        FCmpLe,
        Jump,
        Phi,
        GetElementPtr,
        Call,
        Zext,
        Sext,
        Trunc,
        Fptrunc,
        Fpext,
        Fptosi,
        Fptoui,
        Sitofp,
        Uitofp,
        Bitcast,
        Ptrtoint,
        Inttoptr,
        ShiftLeft,
        LShiftRight,
        AShiftRight,
        And,
        Or,
        IDiv,
        UDiv,
        FDiv,
        IRem,
        URem,
        IMul,
        UMul,
        FMul,
        Switch,
        ExtractValue,
        Count
    };

    Instruction(Opcode opcode, Type* type, std::string name = "") : m_opcode(opcode), Value(name, type, ValueKind::Register) {}
    virtual ~Instruction() = default;

    Opcode getOpcode() const { return m_opcode; }

    Value* getOperand(uint32_t index) const { return m_operands[index]; }
    const std::vector<Value*>& getOperands() const { return m_operands; }
    void addOperand(Value* operand) { m_operands.push_back(operand); operand->addUse(this); }
    void removeOperand(Value* operand) { m_operands.erase(std::find(m_operands.begin(), m_operands.end(), operand)); operand->removeFromUses(this); }
    size_t getNumOperands() const { return m_operands.size(); }

    IR::Block* getParentBlock() const { return m_parentBlock; }

    bool isCmp() const { return m_opcode >= Opcode::ICmpEq && m_opcode <= Opcode::FCmpLe; }
    bool isCast() const { return m_opcode >= Opcode::Zext && m_opcode <= Opcode::Inttoptr; }
    bool hasSideEffect() const;
    bool isJump() const { return m_opcode == Opcode::Jump; }
    bool isTerminator() const { return m_opcode == Opcode::Ret || isJump(); }

protected:
    Opcode m_opcode;
    std::vector<Value*> m_operands;
    IR::Block* m_parentBlock;

friend class Block;
friend class Function;
};

class BinaryOperator : public Instruction {
public:
    BinaryOperator(Opcode opcode, Value* lhs, Value* rhs, std::string name = "") : Instruction(opcode, lhs->getType(), name) {
        assert(lhs->getType() == rhs->getType() && "Expected same type");
        addOperand(lhs);
        addOperand(rhs);
    }

    BinaryOperator(Opcode opcode, Value* lhs, Value* rhs, Type* type, std::string name = "") : Instruction(opcode, type, name) {
        assert(lhs->getType() == rhs->getType() && "Expected same type");
        addOperand(lhs);
        addOperand(rhs);
    }

    Value* getLHS() const { return getOperand(0); }
    Value* getRHS() const { return getOperand(1); }
};

class LoadInstruction : public Instruction {
public:
    LoadInstruction(Value* ptr, std::string name = "") : Instruction(Opcode::Load, nullptr, name) { 
        assert(ptr->getType()->getKind() == Type::TypeKind::Pointer && "Expected pointer type");
        m_type = ((PointerType*)ptr->getType())->getPointee();
        addOperand(ptr);
    }
    Value* getPointer() const { return getOperand(0); }
};

class StoreInstruction : public Instruction {
public:
    StoreInstruction(Value* ptr, Value* value, std::string name = "") : Instruction(Opcode::Store, ptr->getType(), name) {
        assert(ptr->getType()->getKind() == Type::TypeKind::Pointer && "Expected pointer type");
        assert(!value->isConstantArray() && !value->isConstantStruct() && "Cannot store complex values. Use a memcpy or manual copy");
        addOperand(ptr); addOperand(value);
    }
};

class AllocateInstruction : public Instruction {
public:
    AllocateInstruction(Type* type, std::string name = "") : Instruction(Opcode::Allocate, type, name) {}
};

class ReturnInstruction : public Instruction {
public:
    ReturnInstruction(Value* value = nullptr) : Instruction(Opcode::Ret, nullptr, "") { if(value) addOperand(value); }
};

class JumpInstruction : public Instruction {
public:
    JumpInstruction(Block* first, Block* second, Value* value);
    JumpInstruction(Block* first);
};

class PhiInstruction : public Instruction {
public:
    PhiInstruction(Type* type, Value* alloca, std::string name = "") : Instruction(Opcode::Phi, type, name), m_alloca(alloca) {}

    Value* getAlloca() const { return m_alloca; }
protected:
    Value* m_alloca;
};

class GEPInstruction : public Instruction {
public:
    GEPInstruction(Type* type, Value* ptr, const std::vector<Value*>& indices, std::string name = "") : Instruction(Opcode::GetElementPtr, type, name) {
        addOperand(ptr);
        for(auto index : indices) {
            assert(index->getType()->isIntType() && "Expected integer type");
            addOperand(index);
        }
    }
    Value* getPointer() const { return getOperand(0); }
};

class CallInstruction : public Instruction {
public:
    CallInstruction(Type* type, Value* callee, std::string name = "");
    CallInstruction(Type* type, Value* callee, const std::vector<Value*>& args, std::string name = "");

    Value* getCallee() const { return m_operands.at(0); }
    std::span<Value* const> getArguments() const { return { m_operands.data() + 1, m_operands.size() - 1 }; }
};

class CastInstruction : public Instruction {
public:
    CastInstruction(Opcode opcode, Value* value, Type* type, std::string name = "") : Instruction(opcode, type, name) { addOperand(value); }
    Value* getValue() const { return getOperand(0); }
};

class SwitchInstruction : public Instruction {
public:
    SwitchInstruction(Value* value, Block* defaultCase, std::vector<std::pair<ConstantInt*, Block*>> cases);

    Value* getCondition() const { return getOperand(0); }
    Block* getDefaultCase() const;
    std::vector<std::pair<ConstantInt*, Block*>> getCases() const;
};

class ExtractValueInstruction : public Instruction {
public:
    ExtractValueInstruction(Value* aggregate, ConstantInt* index, std::string name = "") : Instruction(Opcode::ExtractValue, aggregate->getType()->getContainedTypes().at(index->getValue()), name) { assert(aggregate->getType()->isStructType()); addOperand(aggregate); addOperand(index); }
    Value* getAggregate() const { return getOperand(0); }
    ConstantInt* getIndex() const;
};

}