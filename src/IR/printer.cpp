#include "IR/printer.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "printer_util.hpp"
#include "type.hpp"
#include "unit.hpp"
#include <ostream>

namespace scbe::IR {

void Printer::printIndentation(std::ostream& os) {
    for(int i = 0; i < m_indent; i++) os << "\t";
}

void HumanPrinter::print(std::ostream& os, Unit& unit) {
    os << "Unit " << unit.getName() << "\n\n";

    for(auto& global : unit.getGlobals()) {
        os << "global ";
        if(global->getName().empty()) os << "(anonymous) ";
        else os << global->getName() << " ";

        if(global->isConstant()) os << "constant ";

        print(os, global->getType());

        if(global->getValue()) {
            os << " = ";
            print(os, global->getValue());
        }
        os << "\n";
    }

    if(!unit.getGlobals().empty()) os << "\n";

    for(auto& function : unit.getFunctions())
        print(os, function.get());

    for(const StructType* deferred : m_deferPrintStruct) {
        os << deferred->getName() << " { ";
        for(size_t i = 0; i < deferred->getContainedTypes().size(); i++) {
            print(os, deferred->getContainedTypes().at(i));
            if(i < deferred->getContainedTypes().size() - 1)
                os << ", ";
        }
        os << " };\n";
    }
    os << "\n";
}

void HumanPrinter::print(std::ostream& os, const Function* function) {
    if(!function->hasBody()) {
        os << "extern ";
    }
    os << "fn " << function->getName();

    os << "(";
    for(int i = 0; i < function->getArguments().size(); i++) {
        print(os, function->getArguments().at(i).get());

        if(i < function->getFunctionType()->getArguments().size() - 1)
            os << ", ";
    }
    os << ")";

    os << " -> ";
    print(os, function->getFunctionType()->getReturnType());

    if(!function->hasBody()) {
        os << ";\n\n";
        return;
    }

    os << " {\n";

    for(auto& block : function->getBlocks()) {
        print(os, block.get());
    }
    
    os << "}\n\n";
}

void HumanPrinter::print(std::ostream& os, const Block* block) {
    printIndentation(os);
    os << block->getName() << ":\n";
    m_indent++;
    for(auto& instruction : block->getInstructions()) {
        print(os, instruction.get());
        os << "\n";
    }
    m_indent--;
}

void HumanPrinter::print(std::ostream& os, const Instruction* instruction) {
    printIndentation(os);

    switch(instruction->getOpcode()) {
        case Instruction::Opcode::Allocate: {
            auto allocateInstruction = static_cast<const AllocateInstruction*>(instruction);
            os << "%" << instruction->getName() << " = allocate ";
            auto typeToPrint = allocateInstruction->getType()->isArrayType() ? allocateInstruction->getType() : ((PointerType*)allocateInstruction->getType())->getPointee();
            print(os, typeToPrint);
            return;
        }
        case Instruction::Opcode::Call: {
            if(instruction->getUses().size() > 0)
                os << "%" << instruction->getName() << " = ";
            os << "call ";
            auto callInstruction = static_cast<const CallInstruction*>(instruction);
            print(os, callInstruction->getCallee());
            os << "(";
            for(int i = 0; i < callInstruction->getArguments().size(); i++) {
                print(os, callInstruction->getArguments()[i]);
                if(i < callInstruction->getArguments().size() - 1)
                    os << ", ";
            }
            os << ")";
            return;
        }
        case Instruction::Opcode::Switch: {
            auto switchInstruction = static_cast<const SwitchInstruction*>(instruction);
            os << "switch ";
            print(os, switchInstruction->getCondition());
            os << " ";
            print(os, (const Value*)switchInstruction->getDefaultCase());
            os << " {\n";
            for(auto& casePair : switchInstruction->getCases()) {
                printIndentation(os);
                printIndentation(os);
                print(os, casePair.first);
                os << " -> ";
                print(os, (const Value*)casePair.second);
                os << "\n";
            }
            printIndentation(os);
            os << "}\n";
            return;
        }
        default:
            break;
    }

    if(!instruction->getName().empty())
        os << "%" << instruction->getName() << " = ";
    
    if(instruction->isCast()) {
        const CastInstruction* castInstruction = static_cast<const CastInstruction*>(instruction);
        print(os, castInstruction->getType());
        os << " ";
    }
    print(os, instruction->getOpcode());
    os << " ";
    for(int i = 0; i < instruction->getNumOperands(); i++) {
        print(os, instruction->getOperand(i));
        if(i < instruction->getNumOperands() - 1)
            os << ", ";
    }
}

void HumanPrinter::print(std::ostream& os, Instruction::Opcode opcode) {
  switch (opcode) {
    case Instruction::Opcode::Load:
        os << "load";
        break;
    case Instruction::Opcode::Store:
        os << "store";
        break;
    case Instruction::Opcode::Add:
        os << "add";
        break;
    case Instruction::Opcode::Sub:
        os << "sub";
        break;
    case Instruction::Opcode::IMul:
        os << "imul";
        break;
    case Instruction::Opcode::UMul:
        os << "umul";
        break;
    case Instruction::Opcode::FMul:
        os << "fmul";
        break;
    case Instruction::Opcode::Allocate:
        os << "allocate";
        break;
    case Instruction::Opcode::Ret:
        os << "ret";
        break;
    case Instruction::Opcode::ICmpEq:
        os << "icmp eq";
        break;
    case Instruction::Opcode::ICmpNe:
        os << "icmp ne";
        break;
    case Instruction::Opcode::ICmpGt:
        os << "icmp gt";
        break;
    case Instruction::Opcode::ICmpGe:
        os << "icmp ge";
        break;
    case Instruction::Opcode::ICmpLt:
        os << "icmp lt";
        break;
    case Instruction::Opcode::ICmpLe:
        os << "icmp le";
        break;
    case Instruction::Opcode::UCmpGt:
        os << "ucmp gt";
        break;
    case Instruction::Opcode::UCmpGe:
        os << "ucmp ge";
        break;
    case Instruction::Opcode::UCmpLt:
        os << "ucmp lt";
        break;
    case Instruction::Opcode::UCmpLe:
        os << "ucmp le";
        break;
    case Instruction::Opcode::FCmpEq:
        os << "fcmp eq";
        break;
    case Instruction::Opcode::FCmpNe:
        os << "fcmp ne";
        break;
    case Instruction::Opcode::FCmpGt:
        os << "fcmp gt";
        break;
    case Instruction::Opcode::FCmpGe:
        os << "fcmp ge";
        break;
    case Instruction::Opcode::FCmpLt:
        os << "fcmp lt";
        break;
    case Instruction::Opcode::FCmpLe:
        os << "fcmp le";
        break;
    case Instruction::Opcode::Jump:
        os << "jump";
        break;
    case Instruction::Opcode::Phi:
        os << "phi";
        break;
    case Instruction::Opcode::GetElementPtr:
        os << "getelementptr";
        break;
    case Instruction::Opcode::Call:
        os << "call";
        break;
    case Instruction::Opcode::Zext:
        os << "zext";
        break;
    case Instruction::Opcode::Sext:
        os << "sext";
        break;
    case Instruction::Opcode::Trunc:
        os << "trunc";
        break;
    case Instruction::Opcode::Fptrunc:
        os << "fptrunc";
        break;
    case Instruction::Opcode::Fpext:
        os << "fpext";
        break;
    case Instruction::Opcode::Fptosi:
        os << "fptosi";
        break;
    case Instruction::Opcode::Uitofp:
        os << "uitofp";
        break;
    case Instruction::Opcode::Bitcast:
        os << "bitcast";
        break;
    case Instruction::Opcode::Ptrtoint:
        os << "ptrtoint";
        break;
    case Instruction::Opcode::Inttoptr:
        os << "inttoptr";
        break;
    case Instruction::Opcode::Fptoui:
        os << "fptoui";
        break;
    case Instruction::Opcode::Sitofp:
        os << "sitofp";
        break;
    case Instruction::Opcode::ShiftLeft:
        os << "shl";
        break;
    case Instruction::Opcode::LShiftRight:
        os << "lshr";
        break;
    case Instruction::Opcode::AShiftRight:
        os << "ashr";
        break;
    case Instruction::Opcode::And:
        os << "and";
        break;
    case Instruction::Opcode::Or:
        os << "or";
        break;
    case Instruction::Opcode::IDiv:
        os << "idiv";
        break;
    case Instruction::Opcode::UDiv:
        os << "udiv";
        break;
    case Instruction::Opcode::FDiv:
        os << "fdiv";
        break;
    case Instruction::Opcode::IRem:
        os << "irem";
        break;
    case Instruction::Opcode::URem:
        os << "urem";
        break;
    case Instruction::Opcode::Switch:
        os << "switch";
        break;
    case Instruction::Opcode::ExtractValue:
        os << "extractvalue";
        break;
    case Instruction::Opcode::Count:
        break;
      break;
    }
}

void HumanPrinter::print(std::ostream& os, const Value* value) {
  switch (value->getKind()) {
    case Value::ValueKind::ConstantInt: {
        print(os, value->getType());
        os << " ";
        auto constantInt = (const ConstantInt*)value;
        os << constantInt->getValue();
        break;
    }
    case Value::ValueKind::ConstantFloat: {
        print(os, value->getType());
        os << " ";
        auto constantFloat = (const ConstantFloat*)value;
        os << constantFloat->getValue();
        break;
    }
    case Value::ValueKind::ConstantString: {
        print(os, value->getType());
        os << " ";
        auto constantString = (const ConstantString*)value;
        os << "\"" << escapeString(constantString->getValue()) << "\"";
        break;
    }
    case Value::ValueKind::ConstantStruct: {
        print(os, value->getType());
        auto constantStruct = (const ConstantStruct*)value;
        os << " { ";
        for (int i = 0; i < constantStruct->getValues().size(); i++) {
            print(os, constantStruct->getValues().at(i));
            if (i != constantStruct->getValues().size() - 1) {
                os << ", ";
            }
        }
        os << " }";
        break;
    }
    case Value::ValueKind::ConstantArray: {
        print(os, value->getType());
        auto constantArray = (const ConstantArray*)value;
        os << " [ ";
        for (int i = 0; i < constantArray->getValues().size(); i++) {
            print(os, constantArray->getValues().at(i));
            if (i != constantArray->getValues().size() - 1) {
                os << ", ";
            }
        }
        os << " ]";
        break;
    }
    case Value::ValueKind::Block:
        os << value->getName();
        break;
    case Value::ValueKind::Function: {
        Function* function = (Function*)value;
        print(os, function->getFunctionType());
        os << " ";
        os << function->getName();
        break;
    }
    case Value::ValueKind::Register:
    case Value::ValueKind::FunctionArgument:
        print(os, value->getType());
        os << " %" << value->getName();
        break;
    case Value::ValueKind::GlobalVariable:
        print(os, value->getType());
        os << " @" << value->getName();
        break;
    case Value::ValueKind::UndefValue:
        print(os, value->getType());
        os << " undef";
        break;
    }
}

void HumanPrinter::print(std::ostream& os, const Type* type) {
  switch (type->getKind()) {
    case Type::TypeKind::Integer: {
        auto intType = (const IntegerType*)type;
        os << "i" << (int)intType->getBits();
        break;
    }
    case Type::TypeKind::Float: {
        auto floatType = (const FloatType*)type;
        os << "f" << (int)floatType->getBits();
        break;
    }
    case Type::TypeKind::Pointer: {
        auto pointerType = (const PointerType*)type;
        print(os, pointerType->getPointee());
        os << "*";
        break;
    }
    case Type::TypeKind::Void: {
        os << "void";
        break;
    }
    case Type::TypeKind::Struct: {
        auto basicType = (const StructType*)type;
        os << basicType->getName();
        m_deferPrintStruct.insert(basicType);
        break;
    }
    case Type::TypeKind::Array: {
        auto arrayType = (const ArrayType*)type;
        print(os, arrayType->getElement());
        os << "[" << arrayType->getScale() << "]";
        break;
    }
    case Type::TypeKind::Function:
        auto fnType = (FunctionType*)type;
        print(os, fnType->getReturnType());
        os << "(";
        for(size_t i = 0; i < fnType->getArguments().size(); i++) {
            print(os, fnType->getArguments()[i]);
            if(i != fnType->getArguments().size() - 1) {
                os << ", ";
            }
        }
        os << ")";
        break;
  }
}
    
}