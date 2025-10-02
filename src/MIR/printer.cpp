#include "MIR/printer.hpp"
#include "IR/function.hpp"
#include "MIR/function.hpp"
#include "MIR/instruction.hpp"
#include "MIR/operand.hpp"
#include "unit.hpp"
#include <stdexcept>

namespace scbe::MIR {

void HumanPrinter::print(Unit& unit) {
    m_unit = &unit;
    m_output << "Unit " << unit.getName() << "\n\n";

    m_output << "\n";

    for(auto& func : unit.getFunctions()) {
        if(!func->hasBody()) continue;
        print(func->getMachineFunction());
    }
}

void HumanPrinter::print(MIR::Function* function) {
    m_current = function;

    m_output << "fn " << function->getName() << " {\n";
    for(auto& block : function->getBlocks()) {
        print(block.get());
    }
    m_output << "}\n\n";
}

void HumanPrinter::print(MIR::Block* block) {
    m_output << block->getName() << ":\n";
    for(auto& instruction : block->getInstructions()) {
        print(instruction.get());
        m_output << "\n";
    }
}

void HumanPrinter::print(MIR::Instruction* instruction) {
    m_output << "    ";

    auto desc = m_instructionInfo->getInstructionDescriptor(instruction->getOpcode());
    
    m_output << desc.getName() << " ";

    for(size_t i = 0; i < desc.getNumOperands(); i++) {
        auto restriction = desc.getRestriction(i);
        print(instruction->getOperands().at(i));
        if(i < desc.getNumOperands() - 1) m_output << ", ";
    }
}

void HumanPrinter::print(MIR::Operand* operand) {
    if(!operand) {
        m_output << "_";
        return;
    }
    switch (operand->getKind()) {
        case Operand::Kind::Register: {
            auto rr = cast<MIR::Register>(operand);
            if(m_registerInfo->isPhysicalRegister(rr->getId())) {
                m_output << m_registerInfo->getRegisterDesc(rr->getId()).getName();
                break;
            }
            m_output << "%" << rr->getId();
            break;
        }
        case Operand::Kind::ImmediateInt: {
            auto imm = cast<MIR::ImmediateInt>(operand);
            m_output << imm->getValue();
            break;
        }
        case Operand::Kind::Block: {
            auto bb = cast<MIR::Block>(operand);
            m_output << bb->getName();
            break;
        }
        case Operand::Kind::GlobalAddress: {
            auto ga = cast<MIR::GlobalAddress>(operand);
            m_output << ga->getName();
            break;
        }
        case Operand::Kind::ExternalSymbol: {
            auto es = cast<MIR::ExternalSymbol>(operand);
            m_output << es->getName();
            break;
        }
        case Operand::Kind::ConstantIndex: {
            auto ci = cast<MIR::ConstantIndex>(operand);
            m_output << ci->getName();
            break;
        }
        default:
            throw std::runtime_error("Unsupported operand kind");
    }
}

}