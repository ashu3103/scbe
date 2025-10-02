#pragma once

#include "MIR/instruction.hpp"
#include "target/register_info.hpp"
#include "target/target_lowering.hpp"

namespace scbe::Target::AArch64 {

class AArch64TargetLowering : public TargetLowering {
public:
    AArch64TargetLowering(RegisterInfo* registerInfo, InstructionInfo* instructionInfo, DataLayout* dataLayout, OS os) : TargetLowering(registerInfo, instructionInfo, dataLayout, os) {}

    void lowerCall(MIR::Block* block, MIR::CallLowering* instruction) override;
    void lowerFunction(MIR::Function* function) override;
    void lowerSwitch(MIR::Block* block, MIR::SwitchLowering* instruction) override;
    void lowerReturn(MIR::Block* block, MIR::ReturnLowering* instruction) override;
};

}