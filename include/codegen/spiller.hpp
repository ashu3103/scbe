#pragma once

#include "MIR/stack_slot.hpp"

namespace scbe {
class DataLayout;
}

namespace scbe::Target {
class InstructionInfo;
class RegisterInfo;
}

namespace scbe::MIR {
class Function;
}

namespace scbe::Codegen {

class Spiller {
public:
    Spiller(DataLayout* dataLayout, Target::InstructionInfo* instrInfo, Target::RegisterInfo* registerInfo) : m_dataLayout(dataLayout), m_instructionInfo(instrInfo), m_registerInfo(registerInfo) {}

    void spill(MIR::Register* replace, MIR::Function* function);
    void spill(MIR::Register* replace, MIR::Function* function, MIR::StackSlot slot);

private:
    DataLayout* m_dataLayout = nullptr;
    Target::InstructionInfo* m_instructionInfo = nullptr;
    Target::RegisterInfo* m_registerInfo = nullptr;
};

}