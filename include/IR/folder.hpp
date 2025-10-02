#pragma once

#include "IR/instruction.hpp"
#include <functional>

namespace scbe::IR {

class Folder {
public:
    Folder(Ref<Context> context) : m_context(context) {}

    Value* foldBinOp(Instruction::Opcode opcode, Value* lhs, Value* rhs);
    Value* foldCast(Instruction::Opcode opcode, Value* value, Type* type);

private:
    template<typename T, typename F>
    Value* foldBinOpInternal(Instruction::Opcode opcode, T* lhs, T* rhs, std::function<T*(F)> constructor);
    
    Value* foldBinOpInternalInt(Instruction::Opcode opcode, ConstantInt* lhs, ConstantInt* rhs);

    template<typename T>
    Value* foldCastInternal(Instruction::Opcode opcode, T* value, Type* type);

private:
    Ref<Context> m_context;
};

}